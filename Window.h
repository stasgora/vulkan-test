#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "components/DebugLayer.h"
#include "components/DeviceManager.h"
#include "components/SwapChain.h"
#include "rendering/Pipeline.h"
#include "buffers/CommandBuffer.h"
#include "rendering/Renderer.h"
#include "buffers/UniformBuffer.h"
#include "components/DescriptorSet.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<vkr::Vertex> vertices = {
		{{-0.5f, -0.5f}, {0.2f, 0.0f, 0.8f}},
		{{0.5f, -0.5f}, {0.0f, 0.4f, 0.9f}},
		{{0.5f, 0.5f}, {0.2f, 0.8f, 1.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.5f, 0.7f}}
};
const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

class Window {
public:
	void run();
	virtual ~Window();

	Window();

private:
	void init();
	void sizeDependentWindowSetup(bool firstTime = true);
	void createInstance();
	void createSurface();

	static void getRequiredExtensions(std::vector<const char *>& extensions);
	static void onWindowResized(GLFWwindow *window, int width, int height);

	void loop();
	void sizeDependentWindowCleanup();
	void cleanup();

	vkr::WindowSize size;
	GLFWwindow* window;
	vk::UniqueInstance instance;
	vk::SurfaceKHR surface;

	vkr::DescriptorSet descriptorSet;
	vkr::Pipeline pipeline;
	vkr::Renderer renderer;

	vkr::CommandBuffer commandBuffer;
	vkr::Buffer<vkr::Vertex> vertexBuffer; //TODO should use a single buffer with offsets for better performance
	vkr::Buffer<uint16_t> indexBuffer;
	vkr::UniformBuffer uniformBuffer;

	vkr::DeviceManager deviceManager;
	vkr::SwapChain swapChain;
	vkr::DebugLayer debugLayer;
};


#endif //TEST_WINDOW_H
