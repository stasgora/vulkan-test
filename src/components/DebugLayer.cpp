#include "DebugLayer.h"

#include <iostream>

void vkr::DebugLayer::init(const VkInstance &instance) {
	if (!enableValidationLayers)
		return;
	vk::DebugUtilsMessengerCreateInfoEXT createInfo;
	createDebugMessengerInfo(createInfo);
	if (CreateDebugUtilsMessengerEXT(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(&createInfo), nullptr, &callback) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}
}

bool vkr::DebugLayer::checkValidationLayerSupport() {
	if (!enableValidationLayers)
		return true;
	auto availableLayers = vk::enumerateInstanceLayerProperties();
	for(auto layerName : validationLayers){
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound)
			return false;
	}
	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vkr::DebugLayer::debugCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                              const VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

VkResult vkr::DebugLayer::CreateDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                       const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
	if (!enableValidationLayers)
		return VK_SUCCESS;
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void vkr::DebugLayer::DestroyDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
	if (!enableValidationLayers)
		return;
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

void vkr::DebugLayer::createDebugMessengerInfo(vk::DebugUtilsMessengerCreateInfoEXT &info) {
	info = vk::DebugUtilsMessengerCreateInfoEXT(
			vk::DebugUtilsMessengerCreateFlagsEXT(),
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			debugCallback, nullptr
	);
}

void vkr::DebugLayer::cleanup(const VkInstance &instance) {
	DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
}
