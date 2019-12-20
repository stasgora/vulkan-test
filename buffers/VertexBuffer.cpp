#include "VertexBuffer.h"

void VertexBuffer::createVertexBuffer(const GPUDeviceManager &deviceManager) {
	Buffer stagingBuffer;
	DeviceMemory stagingBufferMemory;
	const Device &device = *deviceManager.device;
	DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	MemoryPropertyFlags properties = MemoryPropertyFlagBits::eHostVisible | MemoryPropertyFlagBits::eHostCoherent;
	createBuffer(deviceManager, bufferSize, BufferUsageFlagBits::eTransferSrc, properties, stagingBuffer, stagingBufferMemory);
	void* data = device.mapMemory(stagingBufferMemory, 0, bufferSize); //TODO use VulkanMemoryAllocator to allocate memory
	memcpy(data, vertices.data(), (size_t) bufferSize);
	device.unmapMemory(stagingBufferMemory);

	BufferUsageFlags usage = BufferUsageFlagBits::eTransferDst | BufferUsageFlagBits::eVertexBuffer;
	createBuffer(deviceManager, bufferSize, usage, MemoryPropertyFlagBits::eDeviceLocal, buffer, bufferMemory);
	copyBuffer(stagingBuffer, buffer, bufferSize, deviceManager);

	device.destroyBuffer(stagingBuffer);
	device.freeMemory(stagingBufferMemory);
}

void VertexBuffer::createBuffer(const GPUDeviceManager &deviceManager, DeviceSize size, const BufferUsageFlags& usage,
                                const MemoryPropertyFlags& properties, Buffer &buffer, DeviceMemory &memory) {
	const Device &device = *deviceManager.device;
	BufferCreateInfo bufferInfo(BufferCreateFlags(), size, usage, SharingMode::eExclusive);
	try {
		buffer = device.createBuffer(bufferInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create the buffer!");
	}
	MemoryRequirements memRequirements = device.getBufferMemoryRequirements(buffer);
	uint32_t memType = findMemoryType(memRequirements.memoryTypeBits, properties, deviceManager.memoryProperties);
	MemoryAllocateInfo allocInfo(memRequirements.size, memType);
	try {
		memory = device.allocateMemory(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to allocate the buffer memory!");
	}
	device.bindBufferMemory(buffer, memory, 0);
}
