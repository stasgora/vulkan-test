#include <set>
#include "GPUDeviceManager.h"
#include "DebugLayer.h"

void GPUDeviceManager::setupDevice(Instance &instance, SurfaceKHR &surface) {
	pickPhysicalDevice(instance, surface);
	createLogicalDevice(surface);
}

void GPUDeviceManager::createLogicalDevice(SurfaceKHR surface) {
	queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value() };
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		DeviceQueueCreateInfo queueCreateInfo(DeviceQueueCreateFlags(), queueFamily, 1, &queuePriority);
		queueCreateInfos.push_back(queueCreateInfo);
	}

	auto deviceFeatures = PhysicalDeviceFeatures();
	DeviceCreateInfo createInfo(DeviceCreateFlags(), static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	try {
		device = physicalDevice.createDeviceUnique(createInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create logical device!");
	}
	graphicsQueue = device->getQueue(queueFamilyIndices.graphicsFamily.value(), 0);
	presentQueue = device->getQueue(queueFamilyIndices.presentFamily.value(), 0);
}

void GPUDeviceManager::pickPhysicalDevice(Instance &instance, SurfaceKHR surface) {
	auto devices = instance.enumeratePhysicalDevices();
	if (devices.empty())
		throw std::runtime_error("failed to find GPUs with Vulkan support!");

	for (const auto &device : devices) {
		if (isDeviceSuitable(device, surface)) {
			physicalDevice = device;
			break;
		}
	}
	if (!physicalDevice)
		throw std::runtime_error("failed to find a suitable GPU!");
	swapChainSupport = querySwapChainSupport(physicalDevice, surface);
}

bool GPUDeviceManager::isDeviceSuitable(PhysicalDevice device, SurfaceKHR surface) {
	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	bool extensionsSupported = checkDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

SwapChainSupportDetails GPUDeviceManager::querySwapChainSupport(PhysicalDevice device, SurfaceKHR surface) {
	SwapChainSupportDetails details;
	details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
	details.formats = device.getSurfaceFormatsKHR(surface);
	details.presentModes = device.getSurfacePresentModesKHR(surface);
	return details;
}

bool GPUDeviceManager::checkDeviceExtensionSupport(PhysicalDevice device) {
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : device.enumerateDeviceExtensionProperties())
		requiredExtensions.erase(extension.extensionName);
	return requiredExtensions.empty();
}

QueueFamilyIndices GPUDeviceManager::findQueueFamilies(PhysicalDevice device, SurfaceKHR surface) {
	QueueFamilyIndices indices;
	auto queueFamilies = device.getQueueFamilyProperties();

	int i = 0;
	for (const auto &queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & QueueFlagBits::eGraphics)
			indices.graphicsFamily = i;
		if (queueFamily.queueCount > 0 && device.getSurfaceSupportKHR(i, surface))
			indices.presentFamily = i;
		if (indices.isComplete())
			break;
		i++;
	}
	return indices;
}
