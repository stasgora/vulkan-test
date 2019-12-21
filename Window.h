#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "components/DebugLayer.h"
#include "components/GPUDeviceManager.h"
#include "components/SwapChain.h"
#include "rendering/GraphicsPipeline.h"
#include "buffers/GPUCommandBuffer.h"
#include "rendering/GraphicsRenderer.h"

using namespace vk;

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
	UniqueInstance instance;
	SurfaceKHR surface;

	vkr::GraphicsPipeline pipeline;
	vkr::GPUCommandBuffer commandBuffer;
	vkr::GPUBuffer<vkr::Vertex> vertexBuffer; //TODO should use a single buffer with offsets for better performance
	vkr::GPUBuffer<uint16_t> indexBuffer;
	vkr::GraphicsRenderer renderer;

	vkr::GPUDeviceManager deviceManager;
	vkr::SwapChain swapChain;
	vkr::DebugLayer debugLayer;
};


#endif //TEST_WINDOW_H
