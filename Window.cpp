#include "Window.h"
#include <iostream>

using namespace std;

void Window::run() {
	init();
	loop();
	cleanup();
}

void Window::init() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	createInstance();
	debugLayer.init(*instance);
	createSurface();
	deviceManager.setupDevice(*instance, surface);
	setupWindow();
	renderer.setupRendering(*deviceManager.device, swapChain.swapChainImages.size());
}

void Window::setupWindow(bool firstTime) {
	if(!firstTime) {
		deviceManager.device->waitIdle(); //TODO pass old swap chain to the new instead of waiting here
		cleanupWindow();
	}
	WindowSize size;
	glfwGetFramebufferSize(window, &size.width, &size.height);
	swapChain.createSwapChain(deviceManager, surface, size);
	pipeline.setupPipeline(*deviceManager.device, swapChain.swapChainExtent, swapChain.swapChainFormat);
	pipeline.createFrameBuffers(*deviceManager.device, swapChain.swapChainExtent, swapChain.swapChainImageViews);
	commandBuffer.setupCommandBuffer(*deviceManager.device, deviceManager.queueFamilyIndices, swapChain, pipeline);
}

void Window::createSurface() {
	VkSurfaceKHR rawSurface;
	if(glfwCreateWindowSurface(*instance, window, nullptr, &rawSurface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
	surface = rawSurface;
}

void Window::createInstance() {
	if (!debugLayer.checkValidationLayerSupport())
		throw runtime_error("validation layers requested, but not available!");

	ApplicationInfo appInfo("Test", VK_MAKE_VERSION(1, 0, 0),
			"None", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1);

	vector<const char *> extensions;
	getRequiredExtensions(extensions);
	auto createInfo = InstanceCreateInfo(InstanceCreateFlags(), &appInfo, 0,
			nullptr, static_cast<uint32_t>(extensions.size()), extensions.data());

	DebugUtilsMessengerCreateInfoEXT debugInfo;
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		debugLayer.createDebugMessengerInfo(debugInfo);
		createInfo.pNext = &debugInfo;
	}
	try {
		instance = createInstanceUnique(createInfo, nullptr);
	} catch (SystemError &err) {
		throw runtime_error("failed to create instance!");
	}
}

void Window::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		renderer.drawFrame(*deviceManager.device, swapChain, commandBuffer.commandBuffers, deviceManager);
	}
	deviceManager.device->waitIdle();
}

void Window::cleanup() {
	cleanupWindow();

	renderer.cleanup(*deviceManager.device);
	commandBuffer.cleanup(*deviceManager.device);
	instance->destroySurfaceKHR(surface);
	debugLayer.cleanup(*instance);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::cleanupWindow() {
	deviceManager.device->waitIdle();

	pipeline.cleanup(*deviceManager.device);
	commandBuffer.clearBuffers(*deviceManager.device);
	swapChain.cleanup(*deviceManager.device);
}

void Window::getRequiredExtensions(vector<const char *>& extensions) {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}
