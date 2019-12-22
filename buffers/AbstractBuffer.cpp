#include "CommandBuffer.h"
#include "Buffer.h"
#include "AbstractBuffer.h"

void vkr::AbstractBuffer::createBuffer(const DeviceManager &deviceManager, vk::DeviceSize size, const vk::BufferUsageFlags& usage,
                                                            const vk::MemoryPropertyFlags& properties, vk::Buffer &buffer, vk::DeviceMemory &memory) {
	const vk::Device &device = *deviceManager.device;
	vk::BufferCreateInfo bufferInfo(vk::BufferCreateFlags(), size, usage, vk::SharingMode::eExclusive);
	try {
		buffer = device.createBuffer(bufferInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create the buffer!");
	}
	vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(buffer);
	uint32_t memType = findMemoryType(memRequirements.memoryTypeBits, properties, deviceManager.memoryProperties);
	vk::MemoryAllocateInfo allocInfo(memRequirements.size, memType);
	try {
		memory = device.allocateMemory(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to allocate the buffer memory!");
	}
	device.bindBufferMemory(buffer, memory, 0);
}

void vkr::AbstractBuffer::copyBuffer(const vk::Buffer &srcBuffer, const vk::Buffer &dstBuffer, vk::DeviceSize size, const DeviceManager &deviceManager) {
	vk::CommandPool bufferTransferCommandPool;
	const vk::Device &device = *deviceManager.device;
	vkr::CommandBuffer::createCommandPool(bufferTransferCommandPool, vk::CommandPoolCreateFlagBits::eTransient, device, deviceManager.queueFamilyIndices);

	vk::CommandBufferAllocateInfo allocInfo(bufferTransferCommandPool, vk::CommandBufferLevel::ePrimary, 1);
	vk::CommandBuffer commandBuffer = device.allocateCommandBuffers(allocInfo)[0];
	vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	commandBuffer.begin(beginInfo);
	vk::BufferCopy copyRegion(0, 0, size);
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);
	commandBuffer.end();
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	deviceManager.graphicsQueue.submit(1, &submitInfo, nullptr);
	deviceManager.graphicsQueue.waitIdle();

	device.freeCommandBuffers(bufferTransferCommandPool, 1, &commandBuffer);
	device.destroyCommandPool(bufferTransferCommandPool);
}

uint32_t vkr::AbstractBuffer::findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties, const vk::PhysicalDeviceMemoryProperties &memoryProperties) {
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("failed to find suitable memory type!");
}

void vkr::AbstractBuffer::copyBufferData(const vk::Device &device, vk::DeviceMemory &bufferMemory, const void *data, const size_t size) {
	void* bufferData = device.mapMemory(bufferMemory, 0, size); //TODO use VulkanMemoryAllocator to allocate memory
	memcpy(bufferData, data, size);
	device.unmapMemory(bufferMemory);
}