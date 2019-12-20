#include "GPUBuffer.h"
#include "GPUCommandBuffer.h"


void GPUBuffer::copyBuffer(const Buffer &srcBuffer, const Buffer &dstBuffer, DeviceSize size, const GPUDeviceManager &deviceManager) {
	CommandPool bufferTransferCommandPool;
	const Device &device = *deviceManager.device;
	GPUCommandBuffer::createCommandPool(bufferTransferCommandPool, CommandPoolCreateFlagBits::eTransient, device, deviceManager.queueFamilyIndices);

	CommandBufferAllocateInfo allocInfo(bufferTransferCommandPool, CommandBufferLevel::ePrimary, 1);
	CommandBuffer commandBuffer = device.allocateCommandBuffers(allocInfo)[0];
	CommandBufferBeginInfo beginInfo(CommandBufferUsageFlagBits::eOneTimeSubmit);
	commandBuffer.begin(beginInfo);
	BufferCopy copyRegion(0, 0, size);
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);
	commandBuffer.end();
	SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	deviceManager.graphicsQueue.submit(1, &submitInfo, nullptr);
	deviceManager.graphicsQueue.waitIdle();

	device.freeCommandBuffers(bufferTransferCommandPool, 1, &commandBuffer);
	device.destroyCommandPool(bufferTransferCommandPool);
}

uint32_t GPUBuffer::findMemoryType(uint32_t typeFilter, const MemoryPropertyFlags& properties, const PhysicalDeviceMemoryProperties &memoryProperties) {
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("failed to find suitable memory type!");
}

void GPUBuffer::cleanup(const Device &device) {
	device.destroyBuffer(buffer);
	device.freeMemory(bufferMemory);
}
