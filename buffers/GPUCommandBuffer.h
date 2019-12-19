#ifndef TEST_GPUCOMMANDBUFFER_H
#define TEST_GPUCOMMANDBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/VulkanStructs.h"
#include "../components/SwapChain.h"
#include "../rendering/GraphicsPipeline.h"

using namespace vk;

class GPUCommandBuffer {
public:
	void createCommandPool(const Device &device, const QueueFamilyIndices &indices);
	void createCommandBuffer(const Device &device, const SwapChain &swapChain, const GraphicsPipeline &pipeline);
	void cleanup(const Device &device);
	void clearBuffers(const Device &device);

	std::vector<vk::CommandBuffer, std::allocator<vk::CommandBuffer>> commandBuffers;
private:
	CommandPool commandPool;
};


#endif //TEST_GPUCOMMANDBUFFER_H
