#include "CommandBuffer.h"
#include "../components/VulkanStructs.h"
#include "../Window.h"

void vkr::CommandBuffer::createCommandBuffer(const vk::Device &device, const SwapChain &swapChain,
                                             const Pipeline &pipeline, const vk::Buffer &vertexBuffer, const vk::Buffer &indexBuffer) {
	commandBuffers.resize(pipeline.swapChainFramebuffers.size());
	vk::CommandBufferAllocateInfo allocInfo(commandPool, vk::CommandBufferLevel::ePrimary, commandBuffers.size());
	try {
		commandBuffers = device.allocateCommandBuffers(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
	for (int i = 0; i < commandBuffers.size(); ++i) {
		vk::CommandBufferBeginInfo beginInfo;
		try {
			commandBuffers[i].begin(beginInfo);
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		vk::ClearValue clearColor = { std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f } };
		vk::RenderPassBeginInfo renderPassInfo(pipeline.renderPass, pipeline.swapChainFramebuffers[i],
		                                       vk::Rect2D({0, 0}, swapChain.swapChainExtent), 1, &clearColor);
		commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.graphicsPipeline);

		vk::Buffer vertexBuffers[] = {vertexBuffer};
		vk::DeviceSize offsets[] = {0};
		commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);
		commandBuffers[i].bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint16);

		commandBuffers[i].drawIndexed(indices.size(), 1, 0, 0, 0);
		commandBuffers[i].endRenderPass();
		try {
			commandBuffers[i].end();
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void vkr::CommandBuffer::createMainCommandPool(const vk::Device &device, const QueueFamilyIndices &indices) {
	createCommandPool(commandPool, vk::CommandPoolCreateFlags(), device, indices);
}

void vkr::CommandBuffer::cleanup(const vk::Device &device) {
	device.destroyCommandPool(commandPool);
}

void vkr::CommandBuffer::clearBuffers(const vk::Device &device) {
	device.freeCommandBuffers(commandPool, commandBuffers);
	commandBuffers.clear();
}

void vkr::CommandBuffer::createCommandPool(vk::CommandPool &commandPool, const vk::CommandPoolCreateFlags &flags, const vk::Device &device, const QueueFamilyIndices &indices) {
	vk::CommandPoolCreateInfo poolInfo(flags, indices.graphicsFamily.value());
	try {
		commandPool = device.createCommandPool(poolInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create command pool!");
	}
}