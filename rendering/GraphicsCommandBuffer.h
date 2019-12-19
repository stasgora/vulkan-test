#ifndef TEST_GRAPHICSCOMMANDBUFFER_H
#define TEST_GRAPHICSCOMMANDBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/ComponentStructs.h"
#include "../components/SwapChain.h"
#include "GraphicsPipeline.h"

using namespace vk;

class GraphicsCommandBuffer {
public:
	void createCommandPool(const Device &device, const QueueFamilyIndices &indices);
	void createCommandBuffer(const Device &device, const SwapChain &swapChain, const GraphicsPipeline &pipeline);
	void cleanup(const Device &device);
	void clearBuffers(const Device &device);

	std::vector<vk::CommandBuffer, std::allocator<vk::CommandBuffer>> commandBuffers;
private:
	CommandPool commandPool;
};


#endif //TEST_GRAPHICSCOMMANDBUFFER_H
