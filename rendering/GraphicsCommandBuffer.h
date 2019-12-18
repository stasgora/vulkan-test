#ifndef TEST_GRAPHICSCOMMANDBUFFER_H
#define TEST_GRAPHICSCOMMANDBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/ComponentStructs.h"
#include "../components/SwapChain.h"
#include "GraphicsPipeline.h"

using namespace vk;

class GraphicsCommandBuffer {
public:
	void createCommandPool(Device &device, QueueFamilyIndices &indices);
	void createCommandBuffer(Device &device, SwapChain &swapChain, GraphicsPipeline &pipeline);
	void cleanup(Device &device);
	void clearBuffers(Device &device);

	std::vector<vk::CommandBuffer, std::allocator<vk::CommandBuffer>> commandBuffers;
private:
	CommandPool commandPool;
};


#endif //TEST_GRAPHICSCOMMANDBUFFER_H
