#ifndef TEST_GPUCOMMANDBUFFER_H
#define TEST_GPUCOMMANDBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/VulkanStructs.h"
#include "../components/SwapChain.h"
#include "../rendering/GraphicsPipeline.h"
#include "GPUBuffer.h"

using namespace vk;

class GPUCommandBuffer {
public:
	void createMainCommandPool(const Device &device, const QueueFamilyIndices &indices);
	void createCommandBuffer(const Device &device, const SwapChain &swapChain, const GraphicsPipeline &pipeline, const GPUBuffer<Vertex> &vertexBuffer);
	void cleanup(const Device &device);
	void clearBuffers(const Device &device);

	static void createCommandPool(CommandPool &commandPool, const CommandPoolCreateFlags &flags, const Device &device, const QueueFamilyIndices &indices);

	std::vector<vk::CommandBuffer> commandBuffers;
private:
	CommandPool commandPool;
};


#endif //TEST_GPUCOMMANDBUFFER_H
