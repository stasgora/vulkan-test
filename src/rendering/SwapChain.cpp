#include "SwapChain.h"
#include "../components/DeviceManager.h"
#include "../components/VulkanStructs.h"
#include "../image/TextureImage.h"
#include "../image/ImageUtils.h"

void vkr::SwapChain::createSwapChain(const vk::SurfaceKHR &surface, const WindowSize size) {
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

void vkr::SwapChain::createImageViews() {
	swapChainImageViews.reserve(swapChainImages.size());
	for (int i = 0; i < swapChainImages.size(); ++i)
		swapChainImageViews.push_back(ImageUtils::createImageView(device, swapChainImages[i], swapChainFormat, vk::ImageAspectFlagBits::eColor));
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

void vkr::SwapChain::cleanup() {
	for (const auto &imageView : swapChainImageViews)
		device.destroyImageView(imageView, nullptr);
	swapChainImageViews.clear();
	swapChainImages.clear();
	device.destroySwapchainKHR(swapChain, nullptr);
}

vkr::SwapChain::SwapChain(const vkr::DeviceManager &deviceManager) : RendererComponent(deviceManager) {}
