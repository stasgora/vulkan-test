#ifndef TEST_SWAPCHAIN_H
#define TEST_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include "../components/VulkanStructs.h"
#include "../components/DeviceManager.h"
#include "../components/RendererComponent.h"


namespace vkr {
	class SwapChain : public RendererComponent {
	public:
		SwapChain(const DeviceManager &deviceManager);

		void createSwapChain(const vk::SurfaceKHR &surface, const WindowSize size);
		void createImageViews();
		void cleanup() override;

		vk::SwapchainKHR swapChain;
		vk::Extent2D swapChainExtent;
		vk::Format swapChainFormat;
		std::vector<vk::ImageView> swapChainImageViews;
		std::vector<vk::Image> swapChainImages;
	private:
		vk::SwapchainCreateInfoKHR createSwapChainInfo(const SwapChainSupportDetails &swapChainSupport, const QueueFamilyIndices &indices, const vk::SurfaceKHR &surface);

		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities, const WindowSize size);
	};
}


#endif //TEST_SWAPCHAIN_H
