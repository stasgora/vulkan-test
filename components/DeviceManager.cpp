#include <set>
#include "DeviceManager.h"
#include "DebugLayer.h"
#include "VulkanStructs.h"

void vkr::DeviceManager::setupDevice(const vk::Instance &instance, const vk::SurfaceKHR &surface) {
	pickPhysicalDevice(instance, surface);
	createLogicalDevice(surface);
}

void vkr::DeviceManager::createLogicalDevice(const vk::SurfaceKHR &surface) {
	queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value() };
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		vk::DeviceQueueCreateInfo queueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamily, 1, &queuePriority);
		queueCreateInfos.push_back(queueCreateInfo);
	}

	auto deviceFeatures = vk::PhysicalDeviceFeatures();
	vk::DeviceCreateInfo createInfo(vk::DeviceCreateFlags(), static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(vkr::validationLayers.size());
		createInfo.ppEnabledLayerNames = vkr::validationLayers.data();
	}
	try {
		device = physicalDevice.createDeviceUnique(createInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create logical device!");
	}
	graphicsQueue = device->getQueue(queueFamilyIndices.graphicsFamily.value(), 0);
	presentQueue = device->getQueue(queueFamilyIndices.presentFamily.value(), 0);
}

void vkr::DeviceManager::pickPhysicalDevice(const vk::Instance &instance, const vk::SurfaceKHR &surface) {
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
	memoryProperties = physicalDevice.getMemoryProperties();
}

bool vkr::DeviceManager::isDeviceSuitable(const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface) {
	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	bool extensionsSupported = checkDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

vkr::SwapChainSupportDetails vkr::DeviceManager::querySwapChainSupport(const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface) {
	SwapChainSupportDetails details;
	details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
	details.formats = device.getSurfaceFormatsKHR(surface);
	details.presentModes = device.getSurfacePresentModesKHR(surface);
	return details;
}

bool vkr::DeviceManager::checkDeviceExtensionSupport(const vk::PhysicalDevice &device) {
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : device.enumerateDeviceExtensionProperties())
		requiredExtensions.erase(extension.extensionName);
	return requiredExtensions.empty();
}

vkr::QueueFamilyIndices vkr::DeviceManager::findQueueFamilies(const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface) {
	QueueFamilyIndices indices;
	auto queueFamilies = device.getQueueFamilyProperties();

	int i = 0;
	for (const auto &queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			indices.graphicsFamily = i;
		if (queueFamily.queueCount > 0 && device.getSurfaceSupportKHR(i, surface))
			indices.presentFamily = i;
		if (indices.isComplete())
			break;
		i++;
	}
	return indices;
}
