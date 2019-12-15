#ifndef TEST_GRAPHICSRENDERER_H
#define TEST_GRAPHICSRENDERER_H

#include <vulkan/vulkan.hpp>
#include "../components/SwapChain.h"
#include "../components/GPUDeviceManager.h"

using namespace vk;

class GraphicsRenderer {
public:
	void setupRendering(Device &device);
	void cleanup(Device &device);
	void drawFrame(Device &device, SwapChain &swapChain, std::vector<vk::CommandBuffer, std::allocator<vk::CommandBuffer>> &buffers, GPUDeviceManager &deviceManager);

private:
	Semaphore imageAvailableSemaphore;
	Semaphore renderFinishedSemaphore;
};


#endif //TEST_GRAPHICSRENDERER_H
