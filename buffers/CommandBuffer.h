#ifndef TEST_COMMANDBUFFER_H
#define TEST_COMMANDBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/VulkanStructs.h"
#include "../rendering/SwapChain.h"
#include "../rendering/Pipeline.h"
#include "Buffer.h"


namespace vkr {
	class CommandBuffer {
	public:
		void createMainCommandPool(const vk::Device &device, const QueueFamilyIndices &indices);
		void createCommandBuffers(const vk::Device &device, const SwapChain &swapChain, const Pipeline &pipeline, const vk::Buffer &vertexBuffer,
		                          const vk::Buffer &indexBuffer, const std::vector<vk::DescriptorSet> &descriptorSets);
		void cleanup(const vk::Device &device);
		void clearBuffers(const vk::Device &device);

		static void createCommandPool(vk::CommandPool &commandPool, const vk::CommandPoolCreateFlags &flags, const vk::Device &device, const QueueFamilyIndices &indices);

		std::vector<vk::CommandBuffer> commandBuffers;
	private:
		vk::CommandPool commandPool;
	};
}


#endif //TEST_COMMANDBUFFER_H
