#include "Window.h"
#include <iostream>

Window::Window()
: size({WIDTH, HEIGHT}),
vertexBuffer(vertices, vk::BufferUsageFlagBits::eVertexBuffer),
indexBuffer(indices, vk::BufferUsageFlagBits::eIndexBuffer) {}

Window::~Window() {
	cleanup();
}

void Window::run() {
	init();
	loop();
}

void Window::init() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, onWindowResized);

	createInstance();
	debugLayer.init(*instance);
	createSurface();
	deviceManager.setupDevice(*instance, surface);
	vkr::AbstractBuffer::createSingleUsageCommandPool(deviceManager);
	commandBuffer.createMainCommandPool(*deviceManager.device, deviceManager.queueFamilyIndices);
	textureImage.createTextureImage(deviceManager);
	textureImage.createTextureImageView(*deviceManager.device);
	vertexBuffer.createDataBuffer(deviceManager);
	indexBuffer.createDataBuffer(deviceManager);
	descriptorSet.createDescriptorSetLayout(*deviceManager.device);
	sizeDependentWindowSetup();
	renderer.setupRendering(*deviceManager.device, swapChain.swapChainImages.size());
}

void Window::sizeDependentWindowSetup(bool firstTime) {
	while (size.width == 0 || size.height == 0) {
		glfwGetFramebufferSize(window, &size.width, &size.height);
		glfwWaitEvents();
	}
	if(!firstTime) {
		deviceManager.device->waitIdle(); //TODO pass old swap chain to the new instead of waiting here
		sizeDependentWindowCleanup();
	}
	swapChain.createSwapChain(deviceManager, surface, size);
	swapChain.createImageViews(*deviceManager.device);
	pipeline.createRenderPass(*deviceManager.device, swapChain.swapChainFormat);
	pipeline.createGraphicsPipeline(*deviceManager.device, swapChain.swapChainExtent, descriptorSet.descriptorSetLayout);
	pipeline.createFrameBuffers(*deviceManager.device, swapChain.swapChainExtent, swapChain.swapChainImageViews);
	uniformBuffer.createUniformBuffers(deviceManager, swapChain.swapChainImages.size());
	descriptorSet.createDescriptorPool(*deviceManager.device, swapChain.swapChainImages.size());
	descriptorSet.createDescriptorSets(*deviceManager.device, swapChain.swapChainImages.size(), uniformBuffer.uniformBuffers);
	commandBuffer.createCommandBuffers(*deviceManager.device, swapChain, pipeline,
			vertexBuffer.buffer, indexBuffer.buffer, descriptorSet.descriptorSets);
}

void Window::createSurface() {
	VkSurfaceKHR rawSurface;
	if(glfwCreateWindowSurface(*instance, window, nullptr, &rawSurface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
	surface = rawSurface;
}

void Window::createInstance() {
	if (!debugLayer.checkValidationLayerSupport())
		throw std::runtime_error("validation layers requested, but not available!");

	vk::ApplicationInfo appInfo("Test", VK_MAKE_VERSION(1, 0, 0),
			"None", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1);

	std::vector<const char *> extensions;
	getRequiredExtensions(extensions);
	auto createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo, 0,
			nullptr, static_cast<uint32_t>(extensions.size()), extensions.data());

	vk::DebugUtilsMessengerCreateInfoEXT debugInfo;
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(vkr::validationLayers.size());
		createInfo.ppEnabledLayerNames = vkr::validationLayers.data();
		debugLayer.createDebugMessengerInfo(debugInfo);
		createInfo.pNext = &debugInfo;
	}
	try {
		instance = createInstanceUnique(createInfo, nullptr);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create instance!");
	}
}

void Window::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		bool drawn = renderer.drawFrame(deviceManager, swapChain, commandBuffer.commandBuffers, uniformBuffer);
		if (!drawn)
			sizeDependentWindowSetup(false);
	}
	deviceManager.device->waitIdle();
}

void Window::cleanup() {
	sizeDependentWindowCleanup();

	renderer.cleanup(*deviceManager.device);
	commandBuffer.cleanup(*deviceManager.device);
	vertexBuffer.cleanup(*deviceManager.device);
	indexBuffer.cleanup(*deviceManager.device);
	descriptorSet.cleanupLayout(*deviceManager.device);
	textureImage.cleanup(*deviceManager.device);
	vkr::AbstractBuffer::cleanupSingleUsageCommandPool(*deviceManager.device);
	instance->destroySurfaceKHR(surface);
	debugLayer.cleanup(*instance);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::sizeDependentWindowCleanup() {
	deviceManager.device->waitIdle();

	pipeline.cleanup(*deviceManager.device);
	commandBuffer.clearBuffers(*deviceManager.device);
	swapChain.cleanup(*deviceManager.device);
	uniformBuffer.cleanup(*deviceManager.device);
	descriptorSet.cleanup(*deviceManager.device);
}

void Window::getRequiredExtensions(std::vector<const char *>& extensions) {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}

void Window::onWindowResized(GLFWwindow *window, int width, int height) {
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->size = {width, height};
	app->renderer.frameBufferResized = true;
}
