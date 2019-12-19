#ifndef TEST_SWAPCHAIN_H
#define TEST_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include "VulkanStructs.h"
#include "GPUDeviceManager.h"

using namespace vk;

class SwapChain {
public:
	void createSwapChain(const GPUDeviceManager &deviceManager, const SurfaceKHR &surface, const WindowSize size);
	void createImageViews(const Device &device);
	void cleanup(const Device &device);

	SwapchainKHR swapChain;
	Extent2D swapChainExtent;
	Format swapChainFormat;
	std::vector<ImageView> swapChainImageViews;
	std::vector<Image> swapChainImages;
private:
	SwapchainCreateInfoKHR createSwapChainInfo(const SwapChainSupportDetails &swapChainSupport, const QueueFamilyIndices &indices, const SurfaceKHR &surface);

	SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<SurfaceFormatKHR>& availableFormats);
	PresentModeKHR choosePresentMode(const std::vector<PresentModeKHR>& availablePresentModes);
	Extent2D chooseSwapExtent(const SurfaceCapabilitiesKHR &capabilities, const WindowSize size);
};


#endif //TEST_SWAPCHAIN_H
