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
	void setupDevice(const Instance& instance, const SurfaceKHR &surface);
	static SwapChainSupportDetails querySwapChainSupport(const PhysicalDevice &device, const SurfaceKHR &surface);

	UniqueDevice device;

	QueueFamilyIndices queueFamilyIndices;
	Queue graphicsQueue;
	Queue presentQueue;
	PhysicalDevice physicalDevice;
private:
	void pickPhysicalDevice(const Instance &instance, const SurfaceKHR &surface);
	void createLogicalDevice(const SurfaceKHR &surface);

	static bool checkDeviceExtensionSupport(const PhysicalDevice &device);
	static bool isDeviceSuitable(const PhysicalDevice &device, const SurfaceKHR &surface);
	static QueueFamilyIndices findQueueFamilies(const PhysicalDevice &device, const SurfaceKHR &surface);
};


#endif //TEST_GPUDEVICEMANAGER_H
