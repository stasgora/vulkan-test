#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "components/DebugLayer.h"
#include "components/GPUDeviceManager.h"
#include "components/SwapChain.h"
#include "rendering/GraphicsPipeline.h"
#include "rendering/GraphicsCommandBuffer.h"
#include "rendering/GraphicsRenderer.h"

using namespace vk;

const int WIDTH = 800;
const int HEIGHT = 600;


class Window {
public:
	void run();

private:
	void init();
	void setupWindow(bool firstTime = true);
	void createInstance();
	void createSurface();

	static void getRequiredExtensions(std::vector<const char *>& extensions);

	void loop();
	void cleanupWindow();
	void cleanup();

	GLFWwindow* window;
	UniqueInstance instance;
	SurfaceKHR surface;

	GraphicsPipeline pipeline;
	GraphicsCommandBuffer commandBuffer;
	GraphicsRenderer renderer;

	GPUDeviceManager deviceManager;
	SwapChain swapChain;
	DebugLayer debugLayer;
};


#endif //TEST_WINDOW_H
