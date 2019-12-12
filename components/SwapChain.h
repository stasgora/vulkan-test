#ifndef TEST_SWAPCHAIN_H
#define TEST_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include "structs.h"

using namespace vk;

class SwapChain {
public:
	void createSwapChain(SwapChainSupportDetails &swapChainSupport, QueueFamilyIndices &queueFamilyIndices, Device &device, SurfaceKHR surface);

	SwapchainKHR swapChain;
private:
	void createImageViews();
	SwapchainCreateInfoKHR createSwapChainInfo(SwapChainSupportDetails &swapChainSupport, QueueFamilyIndices &indices, SurfaceKHR surface);

	SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<SurfaceFormatKHR>& availableFormats);
	PresentModeKHR choosePresentMode(const std::vector<PresentModeKHR>& availablePresentModes);
	Extent2D chooseSwapExtent(const SurfaceCapabilitiesKHR &capabilities);

	std::vector<Image> swapChainImages;
	std::vector<ImageView> swapChainImageViews;
	Format swapChainFormat;
	Extent2D swapChainExtent;
};


#endif //TEST_SWAPCHAIN_H
