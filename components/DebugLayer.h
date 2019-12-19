#ifndef TEST_DEBUGLAYER_H
#define TEST_DEBUGLAYER_H

#include <vulkan/vulkan.hpp>
using namespace vk;

const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
};
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class DebugLayer {
public:
	void init(const VkInstance &instance);
	void cleanup(const VkInstance &instance);

	bool checkValidationLayerSupport();
	void createDebugMessengerInfo(DebugUtilsMessengerCreateInfoEXT& info);

private:
	static VkResult CreateDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	                                             const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);
	static void DestroyDebugUtilsMessengerEXT(const VkInstance &instance, const VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, const VkDebugUtilsMessageTypeFlagsEXT messageType,
	                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	VkDebugUtilsMessengerEXT callback;
};


#endif //TEST_DEBUGLAYER_H
