#include "SwapChain.h"
#include "DeviceManager.h"
#include "VulkanStructs.h"

void vkr::SwapChain::createSwapChain(const vkr::DeviceManager &deviceManager, const vk::SurfaceKHR &surface, const WindowSize size) {
	SwapChainSupportDetails swapChainSupport = vkr::DeviceManager::querySwapChainSupport(deviceManager.physicalDevice, surface);
	swapChainExtent = chooseSwapExtent(swapChainSupport.capabilities, size);
	vk::SwapchainCreateInfoKHR createInfo = createSwapChainInfo(swapChainSupport, deviceManager.queueFamilyIndices, surface);
	vk::Device device = *deviceManager.device;
	try {
		swapChain = device.createSwapchainKHR(createInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create swap chain!");
	}
	swapChainImages = device.getSwapchainImagesKHR(swapChain);
	swapChainFormat = createInfo.imageFormat;
}

vk::SwapchainCreateInfoKHR vkr::SwapChain::createSwapChainInfo(const SwapChainSupportDetails &swapChainSupport, const QueueFamilyIndices& indices, const vk::SurfaceKHR &surface) {
	const vk::PresentModeKHR presentMode = choosePresentMode(swapChainSupport.presentModes);
	const vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0)
		imageCount = std::min(imageCount, swapChainSupport.capabilities.maxImageCount);
	vk::SwapchainCreateInfoKHR createInfo(vk::SwapchainCreateFlagsKHR(), surface, imageCount, surfaceFormat.format,
	                                  surfaceFormat.colorSpace, swapChainExtent, 1, vk::ImageUsageFlagBits::eColorAttachment); // Image post-processing

	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);
	return createInfo;
}

void vkr::SwapChain::createImageViews(const vk::Device &device) {
	swapChainImageViews.reserve(swapChainImages.size());
	for (int i = 0; i < swapChainImages.size(); ++i) {
		vk::ImageViewCreateInfo createInfo(vk::ImageViewCreateFlags(), swapChainImages[i], vk::ImageViewType::e2D, swapChainFormat);
		createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.layerCount = 1;
		try {
			swapChainImageViews.push_back(device.createImageView(createInfo));
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}

vk::SurfaceFormatKHR vkr::SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
	for (const auto& availableFormat : availableFormats)
		if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			return availableFormat;
	return availableFormats[0];
}

vk::PresentModeKHR vkr::SwapChain::choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
	vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;
	for (const auto &mode : availablePresentModes)
		if (mode == vk::PresentModeKHR::eMailbox)
			return mode;
	return bestMode;
}

vk::Extent2D vkr::SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities, const WindowSize size) {
	if (capabilities.currentExtent.width != UINT32_MAX)
		return capabilities.currentExtent;
	vk::Extent2D actualExtent = {static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height)};
	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
	return actualExtent;
}

void vkr::SwapChain::cleanup(const vk::Device &device) {
	for (const auto &imageView : swapChainImageViews)
		device.destroyImageView(imageView);
	swapChainImageViews.clear();
	swapChainImages.clear();
	device.destroySwapchainKHR(swapChain);
}
