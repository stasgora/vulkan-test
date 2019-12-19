#ifndef TEST_GRAPHICSRENDERER_H
#define TEST_GRAPHICSRENDERER_H

#include <vulkan/vulkan.hpp>
#include "../components/SwapChain.h"
#include "../components/GPUDeviceManager.h"

using namespace vk;

const int MAX_FRAMES_IN_FLIGHT = 2;

class GraphicsRenderer {
public:
	void setupRendering(Device &device, uint32_t swapImageCount);
	void cleanup(Device &device);
	bool drawFrame(Device &device, SwapChain &swapChain, std::vector<vk::CommandBuffer, std::allocator<vk::CommandBuffer>> &buffers, GPUDeviceManager &deviceManager);

	bool frameBufferResized = false;
private:
	std::vector<Semaphore> imageAvailableSemaphores;
	std::vector<Semaphore> renderFinishedSemaphores;
	std::vector<Fence> inFlightFences;
	std::vector<Fence*> imagesInFlight;
	size_t currentFrame = 0;
};


#endif //TEST_GRAPHICSRENDERER_H
