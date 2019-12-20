#include "GPUCommandBuffer.h"
#include "../components/VulkanStructs.h"
#include "../Window.h"

void GPUCommandBuffer::createCommandBuffer(const Device &device, const SwapChain &swapChain, const GraphicsPipeline &pipeline, const Buffer &vertexBuffer) {
	commandBuffers.resize(pipeline.swapChainFramebuffers.size());
	CommandBufferAllocateInfo allocInfo(commandPool, CommandBufferLevel::ePrimary, commandBuffers.size());
	try {
		commandBuffers = device.allocateCommandBuffers(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
	for (int i = 0; i < commandBuffers.size(); ++i) {
		CommandBufferBeginInfo beginInfo;
		try {
			commandBuffers[i].begin(beginInfo);
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		ClearValue clearColor = { std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f } };
		RenderPassBeginInfo renderPassInfo(pipeline.renderPass, pipeline.swapChainFramebuffers[i],
				Rect2D({0, 0}, swapChain.swapChainExtent), 1, &clearColor);
		commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		commandBuffers[i].bindPipeline(PipelineBindPoint::eGraphics, pipeline.graphicsPipeline);

		Buffer vertexBuffers[] = {vertexBuffer};
		DeviceSize offsets[] = {0};
		commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);

		commandBuffers[i].draw(vertices.size(), 1, 0, 0);
		commandBuffers[i].endRenderPass();
		try {
			commandBuffers[i].end();
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void GPUCommandBuffer::createMainCommandPool(const Device &device, const QueueFamilyIndices &indices) {
	createCommandPool(commandPool, CommandPoolCreateFlags(), device, indices);
}

void GPUCommandBuffer::cleanup(const Device &device) {
	device.destroyCommandPool(commandPool);
}

void GPUCommandBuffer::clearBuffers(const Device &device) {
	device.freeCommandBuffers(commandPool, commandBuffers);
	commandBuffers.clear();
}

void GPUCommandBuffer::createCommandPool(CommandPool &commandPool, const CommandPoolCreateFlags &flags, const Device &device, const QueueFamilyIndices &indices) {
	CommandPoolCreateInfo poolInfo(flags, indices.graphicsFamily.value());
	try {
		commandPool = device.createCommandPool(poolInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create command pool!");
	}
}
