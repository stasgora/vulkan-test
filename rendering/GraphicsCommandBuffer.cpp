#include "GraphicsCommandBuffer.h"
#include "../components/ComponentStructs.h"

void GraphicsCommandBuffer::setupCommandBuffer(Device &device, QueueFamilyIndices &indices, SwapChain &swapChain, GraphicsPipeline &pipeline) {
	createCommandPool(device, indices, pipeline.swapChainFramebuffers);
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
		commandBuffers[i].draw(3, 1, 0, 0);
		commandBuffers[i].endRenderPass();
		try {
			commandBuffers[i].end();
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void GraphicsCommandBuffer::createCommandPool(Device &device, QueueFamilyIndices &indices, std::vector<Framebuffer> &frameBuffers) {
	CommandPoolCreateInfo poolInfo(CommandPoolCreateFlags(), indices.graphicsFamily.value());
	try {
		commandPool = device.createCommandPool(poolInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create command pool!");
	}
	commandBuffers.resize(frameBuffers.size());
	CommandBufferAllocateInfo allocInfo(commandPool, CommandBufferLevel::ePrimary, commandBuffers.size());
	try {
		commandBuffers = device.allocateCommandBuffers(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void GraphicsCommandBuffer::cleanup(Device &device) {
	device.destroyCommandPool(commandPool);
}