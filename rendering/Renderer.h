#ifndef TEST_RENDERER_H
#define TEST_RENDERER_H

#include <vulkan/vulkan.hpp>
#include "../components/SwapChain.h"
#include "../components/DeviceManager.h"
#include "../buffers/UniformBuffer.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace vkr {
	class Renderer {
	public:
		void setupRendering(const vk::Device &device, uint32_t swapImageCount);
		void cleanup(const vk::Device &device);
		bool
		drawFrame(const DeviceManager &deviceManager, const SwapChain &swapChain, std::vector<vk::CommandBuffer,
				std::allocator<vk::CommandBuffer>> &buffers, vkr::UniformBuffer &uniformBuffer);

		bool frameBufferResized = false;
	private:
		std::vector<vk::Semaphore> imageAvailableSemaphores;
		std::vector<vk::Semaphore> renderFinishedSemaphores;
		std::vector<vk::Fence> inFlightFences;
		std::vector<vk::Fence*> imagesInFlight;
		size_t currentFrame = 0;
	};
}


#endif //TEST_RENDERER_H
