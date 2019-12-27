#include "Window.h"
#include <iostream>

Window::Window()
: size({WIDTH, HEIGHT}),
  vertexBuffer(deviceManager, objModel.vertices, vk::BufferUsageFlagBits::eVertexBuffer),
  indexBuffer(deviceManager, objModel.indices, vk::BufferUsageFlagBits::eIndexBuffer),
  textureImage(deviceManager, "../assets/chalet.jpg", vk::ImageLayout::eShaderReadOnlyOptimal),
  depthImage(deviceManager),
  swapChain(deviceManager),
  pipeline(deviceManager),
  descriptorSet(deviceManager),
  uniformBuffer(deviceManager),
  commandBuffer(deviceManager),
  renderer(deviceManager),
  objModel("../assets/chalet.obj") {}

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
	vkr::BufferUtils::createSingleUsageCommandPool(deviceManager);
	commandBuffer.createMainCommandPool();
	textureImage.init();
	objModel.loadModel();
	vertexBuffer.createDataBuffer();
	indexBuffer.createDataBuffer();
	descriptorSet.createDescriptorSetLayout();
	sizeDependentWindowSetup();
	renderer.setupRendering(swapChain.swapChainImages.size());
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
	swapChain.createSwapChain(surface, size);
	swapChain.createImageViews();
	pipeline.createRenderPass(swapChain.swapChainFormat);
	pipeline.createGraphicsPipeline(swapChain.swapChainExtent, descriptorSet.descriptorSetLayout);
	depthImage.createDepthResources(swapChain.swapChainExtent);
	pipeline.createFrameBuffers(swapChain, depthImage);
	uniformBuffer.createUniformBuffers(swapChain.swapChainImages.size());
	descriptorSet.createDescriptorPool(swapChain.swapChainImages.size());
	descriptorSet.createDescriptorSets(swapChain.swapChainImages.size(), uniformBuffer.uniformBuffers, textureImage);
	commandBuffer.createCommandBuffers(swapChain, pipeline, vertexBuffer, indexBuffer, descriptorSet.descriptorSets);
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
		bool drawn = renderer.drawFrame(swapChain, commandBuffer.commandBuffers, uniformBuffer);
		if (!drawn)
			sizeDependentWindowSetup(false);
	}
	deviceManager.device->waitIdle();
}

void Window::cleanup() {
	sizeDependentWindowCleanup();

	renderer.cleanup();
	commandBuffer.cleanup();
	vertexBuffer.cleanup();
	indexBuffer.cleanup();
	descriptorSet.cleanupLayout();
	textureImage.cleanup();
	vkr::BufferUtils::cleanupSingleUsageCommandPool(*deviceManager.device);
	instance->destroySurfaceKHR(surface);
	debugLayer.cleanup(*instance);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::sizeDependentWindowCleanup() {
	deviceManager.device->waitIdle();

	depthImage.cleanup();
	pipeline.cleanup();
	commandBuffer.clearBuffers();
	swapChain.cleanup();
	uniformBuffer.cleanup();
	descriptorSet.cleanup();
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
