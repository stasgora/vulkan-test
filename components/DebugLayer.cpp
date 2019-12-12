#include "DebugLayer.h"

#include <iostream>
using namespace std;

void DebugLayer::init(VkInstance instance) {
	if (!enableValidationLayers)
		return;
	DebugUtilsMessengerCreateInfoEXT createInfo;
	createDebugMessengerInfo(createInfo);
	if (CreateDebugUtilsMessengerEXT(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(&createInfo), nullptr, &callback) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}
}

bool DebugLayer::checkValidationLayerSupport() {
	if (!enableValidationLayers)
		return true;
	auto availableLayers = enumerateInstanceLayerProperties();
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

VKAPI_ATTR VkBool32 VKAPI_CALL DebugLayer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                     const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	cerr << "validation layer: " << pCallbackData->pMessage << endl;
	return VK_FALSE;
}

VkResult DebugLayer::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
	if (!enableValidationLayers)
		return VK_SUCCESS;
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DebugLayer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
	if (!enableValidationLayers)
		return;
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

void DebugLayer::createDebugMessengerInfo(DebugUtilsMessengerCreateInfoEXT &info) {
	info = DebugUtilsMessengerCreateInfoEXT(
			DebugUtilsMessengerCreateFlagsEXT(),
			DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | DebugUtilsMessageSeverityFlagBitsEXT::eWarning | DebugUtilsMessageSeverityFlagBitsEXT::eError,
			DebugUtilsMessageTypeFlagBitsEXT::eGeneral | DebugUtilsMessageTypeFlagBitsEXT::eValidation | DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			debugCallback, nullptr
	);
}

void DebugLayer::cleanup(VkInstance instance) {
	DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
}
