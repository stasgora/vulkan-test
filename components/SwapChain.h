#ifndef TEST_SWAPCHAIN_H
#define TEST_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include "ComponentStructs.h"

using namespace vk;

class SwapChain {
public:
	void createSwapChain(SwapChainSupportDetails &swapChainSupport, QueueFamilyIndices &queueFamilyIndices, Device &device, SurfaceKHR surface);
	void cleanup(Device &device);

	Extent2D swapChainExtent;
	Format swapChainFormat;
private:
	void createImageViews(Device &device);
	SwapchainCreateInfoKHR createSwapChainInfo(SwapChainSupportDetails &swapChainSupport, QueueFamilyIndices &indices, SurfaceKHR surface);

	SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<SurfaceFormatKHR>& availableFormats);
	PresentModeKHR choosePresentMode(const std::vector<PresentModeKHR>& availablePresentModes);
	Extent2D chooseSwapExtent(const SurfaceCapabilitiesKHR &capabilities);

	SwapchainKHR swapChain;
	std::vector<Image> swapChainImages;
	std::vector<ImageView> swapChainImageViews;
};


#endif //TEST_SWAPCHAIN_H
