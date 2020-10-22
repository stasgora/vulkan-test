#include "CommandBuffer.h"
#include "../components/VulkanStructs.h"
#include "../Window.h"

void vkr::CommandBuffer::createCommandBuffers(const SwapChain &swapChain, const Pipeline &pipeline, const Buffer &dataBuffer,
											  const std::vector<Object> &objects, uint32_t indexOffset, const std::vector<vk::DescriptorSet> &descriptorSets) {
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
		std::array<vk::ClearValue, 2> clearValues = {};
		clearValues[0].color = std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f};
		vk::ClearDepthStencilValue stencilValue(1.0f, 0);
		clearValues[1].depthStencil = stencilValue;
		vk::RenderPassBeginInfo renderPassInfo(pipeline.renderPass, pipeline.swapChainFramebuffers[i],
		                                       vk::Rect2D({0, 0}, swapChain.swapChainExtent), clearValues.size(), clearValues.data());
		commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.graphicsPipeline);


		vk::Buffer vertexBuffers[] = {dataBuffer.buffer};
		vk::DeviceSize offsets[] = {0};
		commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);
		commandBuffers[i].bindIndexBuffer(dataBuffer.buffer, indexOffset, vk::IndexType::eUint32);
		for (const auto &object : objects) {
			commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline.pipelineLayout,
			                                     0, 1, &descriptorSets[i], 0, nullptr);
			commandBuffers[i].drawIndexed(object.indexCount, 1, object.indexBase, object.vertexBase, 0);
		}
		commandBuffers[i].endRenderPass();
		try {
			commandBuffers[i].end();
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void vkr::CommandBuffer::createMainCommandPool() {
	createCommandPool(commandPool, vk::CommandPoolCreateFlags(), device, deviceManager.queueFamilyIndices);
}

void vkr::CommandBuffer::cleanup() {
	device.destroyCommandPool(commandPool, nullptr);
}

void vkr::CommandBuffer::clearBuffers() {
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

vkr::CommandBuffer::CommandBuffer(const vkr::DeviceManager &deviceManager) : RendererComponent(deviceManager) {}
