#ifndef TEST_DEVICEMANAGER_H
#define TEST_DEVICEMANAGER_H

#include <vulkan/vulkan.hpp>
#include "VulkanStructs.h"


namespace vkr {
	const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	class DeviceManager {
	public:
		void setupDevice(const vk::Instance& instance, const vk::SurfaceKHR &surface);
		static SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface);

		vk::UniqueDevice device;

		QueueFamilyIndices queueFamilyIndices;
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
		vk::PhysicalDevice physicalDevice;
		vk::PhysicalDeviceMemoryProperties memoryProperties;

	private:
		void pickPhysicalDevice(const vk::Instance &instance, const vk::SurfaceKHR &surface);
		void createLogicalDevice(const vk::SurfaceKHR &surface);

		static bool checkDeviceExtensionSupport(const vk::PhysicalDevice &device);
		static bool isDeviceSuitable(const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface);
		static QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface);
	};
}


#endif //TEST_DEVICEMANAGER_H
