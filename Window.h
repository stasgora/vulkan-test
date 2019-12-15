#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "components/DebugLayer.h"
#include "components/GPUDeviceManager.h"
#include "components/SwapChain.h"
#include "pipeline/GraphicsPipeline.h"

using namespace vk;

const int WIDTH = 800;
const int HEIGHT = 600;


class Window {
public:
	void run();

private:
	void init();
	void createInstance();
	void createSurface();

	std::vector<const char*> getRequiredExtensions();

	void loop();
	void cleanup();

	GLFWwindow* window;
	UniqueInstance instance;
	SurfaceKHR surface;

	GraphicsPipeline pipeline;
	DebugLayer debugLayer;
	GPUDeviceManager deviceManager;
	SwapChain swapChain;
};


#endif //TEST_WINDOW_H
