#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "components/DebugLayer.h"
#include "components/DeviceManager.h"
#include "rendering/SwapChain.h"
#include "rendering/Pipeline.h"
#include "buffers/CommandBuffer.h"
#include "rendering/Renderer.h"
#include "buffers/UniformBuffer.h"
#include "components/DescriptorSet.h"
#include "image/SampledTexture.h"
#include "image/DepthImage.h"
#include "model/ObjModel.h"

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

	vkr::WindowSize size;
	GLFWwindow* window{};
	vk::UniqueInstance instance;
	vk::SurfaceKHR surface;
	vkr::SampledTexture textureImage;
	vkr::DepthImage depthImage;
	vkr::ObjModel objModel;

	vkr::DescriptorSet descriptorSet;
	vkr::Pipeline pipeline;
	vkr::Renderer renderer;

	vkr::CommandBuffer commandBuffer;
	vkr::Buffer<vkr::Vertex> vertexBuffer; //TODO should use a single buffer with offsets for better performance
	vkr::Buffer<uint32_t> indexBuffer;
	vkr::UniformBuffer uniformBuffer;

	vkr::DeviceManager deviceManager;
	vkr::SwapChain swapChain;
	vkr::DebugLayer debugLayer{};
};


#endif //TEST_WINDOW_H
