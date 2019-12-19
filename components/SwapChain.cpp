#include "SwapChain.h"

void SwapChain::createSwapChain(const GPUDeviceManager &deviceManager, const SurfaceKHR &surface, const WindowSize size) {
	SwapChainSupportDetails swapChainSupport = GPUDeviceManager::querySwapChainSupport(deviceManager.physicalDevice, surface);
	swapChainExtent = chooseSwapExtent(swapChainSupport.capabilities, size);
	SwapchainCreateInfoKHR createInfo = createSwapChainInfo(swapChainSupport, deviceManager.queueFamilyIndices, surface);
	Device device = *deviceManager.device;
	try {
		swapChain = device.createSwapchainKHR(createInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create swap chain!");
	}
	swapChainImages = device.getSwapchainImagesKHR(swapChain);
	swapChainFormat = createInfo.imageFormat;
}

SwapchainCreateInfoKHR SwapChain::createSwapChainInfo(const SwapChainSupportDetails &swapChainSupport, const QueueFamilyIndices& indices, const SurfaceKHR &surface) {
	const PresentModeKHR presentMode = choosePresentMode(swapChainSupport.presentModes);
	const SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0)
		imageCount = std::min(imageCount, swapChainSupport.capabilities.maxImageCount);
	SwapchainCreateInfoKHR createInfo(SwapchainCreateFlagsKHR(), surface, imageCount, surfaceFormat.format,
	                                  surfaceFormat.colorSpace, swapChainExtent, 1, ImageUsageFlagBits::eColorAttachment); // Image post-processing

	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else
		createInfo.imageSharingMode = SharingMode::eExclusive;
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = SwapchainKHR(nullptr);
	return createInfo;
}

void SwapChain::createImageViews(const Device &device) {
	swapChainImageViews.reserve(swapChainImages.size());
	for (int i = 0; i < swapChainImages.size(); ++i) {
		ImageViewCreateInfo createInfo(ImageViewCreateFlags(), swapChainImages[i], ImageViewType::e2D, swapChainFormat);
		createInfo.subresourceRange.aspectMask = ImageAspectFlagBits::eColor;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.layerCount = 1;
		try {
			swapChainImageViews.push_back(device.createImageView(createInfo));
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}

SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<SurfaceFormatKHR> &availableFormats) {
	for (const auto& availableFormat : availableFormats)
		if (availableFormat.format == Format::eB8G8R8A8Unorm && availableFormat.colorSpace == ColorSpaceKHR::eSrgbNonlinear)
			return availableFormat;
	return availableFormats[0];
}

PresentModeKHR SwapChain::choosePresentMode(const std::vector<PresentModeKHR> &availablePresentModes) {
	PresentModeKHR bestMode = PresentModeKHR::eFifo;
	for (const auto &mode : availablePresentModes)
		if (mode == PresentModeKHR::eMailbox)
			return mode;
	return bestMode;
}

Extent2D SwapChain::chooseSwapExtent(const SurfaceCapabilitiesKHR &capabilities, const WindowSize size) {
	if (capabilities.currentExtent.width != UINT32_MAX)
		return capabilities.currentExtent;
	Extent2D actualExtent = {static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height)};
	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
	return actualExtent;
}

void SwapChain::cleanup(const Device &device) {
	for (const auto &imageView : swapChainImageViews)
		device.destroyImageView(imageView);
	swapChainImageViews.clear();
	swapChainImages.clear();
	device.destroySwapchainKHR(swapChain);
}
