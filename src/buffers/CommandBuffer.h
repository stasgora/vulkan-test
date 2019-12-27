#ifndef TEST_COMMANDBUFFER_H
#define TEST_COMMANDBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/VulkanStructs.h"
#include "../rendering/SwapChain.h"
#include "../rendering/Pipeline.h"
#include "Buffer.h"


namespace vkr {
	class CommandBuffer : public RendererComponent {
	public:
		explicit CommandBuffer(const DeviceManager &deviceManager);

		void createMainCommandPool();
		void createCommandBuffers(const SwapChain &swapChain, const Pipeline &pipeline, const Buffer<Vertex> &vertexBuffer,
		                          const Buffer<uint32_t> &indexBuffer, const std::vector<vk::DescriptorSet> &descriptorSets);
		void cleanup() override;
		void clearBuffers();

		static void createCommandPool(vk::CommandPool &commandPool, const vk::CommandPoolCreateFlags &flags, const vk::Device &device, const QueueFamilyIndices &indices);

		std::vector<vk::CommandBuffer> commandBuffers;
	private:
		vk::CommandPool commandPool;
	};
}


#endif //TEST_COMMANDBUFFER_H
