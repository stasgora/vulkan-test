#ifndef TEST_GPUDEVICEMANAGER_H
#define TEST_GPUDEVICEMANAGER_H

#include <vulkan/vulkan.hpp>
#include "ComponentStructs.h"

using namespace vk;

const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class GPUDeviceManager {
public:
	void setupDevice(Instance& instance, SurfaceKHR &surface);

	UniqueDevice device;

	SwapChainSupportDetails swapChainSupport;
	QueueFamilyIndices queueFamilyIndices;
private:
	void pickPhysicalDevice(Instance &instance, SurfaceKHR surface);
	void createLogicalDevice(SurfaceKHR surface);

	SwapChainSupportDetails querySwapChainSupport(PhysicalDevice device, SurfaceKHR surface);
	bool checkDeviceExtensionSupport(PhysicalDevice device);
	bool isDeviceSuitable(PhysicalDevice device, SurfaceKHR surface);
	QueueFamilyIndices findQueueFamilies(PhysicalDevice device, SurfaceKHR surface);

	PhysicalDevice physicalDevice;
	Queue graphicsQueue;
	Queue presentQueue;
};


#endif //TEST_GPUDEVICEMANAGER_H
