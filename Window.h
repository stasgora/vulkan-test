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
#include "buffers/GPUVertexBuffer.h"

using namespace vk;

const int WIDTH = 800;
const int HEIGHT = 600;


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

	WindowSize size;
	GLFWwindow* window;
	UniqueInstance instance;
	SurfaceKHR surface;

	GraphicsPipeline pipeline;
	GPUCommandBuffer commandBuffer;
	GPUVertexBuffer vertexBuffer;
	GraphicsRenderer renderer;

	GPUDeviceManager deviceManager;
	SwapChain swapChain;
	DebugLayer debugLayer;
};


#endif //TEST_WINDOW_H
