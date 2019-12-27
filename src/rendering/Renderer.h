#ifndef TEST_RENDERER_H
#define TEST_RENDERER_H

#include <vulkan/vulkan.hpp>
#include "SwapChain.h"
#include "../components/DeviceManager.h"
#include "../buffers/UniformBuffer.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace vkr {
	class Renderer : public RendererComponent {
	public:
		explicit Renderer(const DeviceManager &deviceManager);

		void setupRendering(uint32_t swapImageCount);
		void cleanup() override;
		bool
		drawFrame(const SwapChain &swapChain, std::vector<vk::CommandBuffer,
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
