#include "GPUVertexBuffer.h"

void GPUVertexBuffer::createVertexBuffer(const Device &device, const PhysicalDeviceMemoryProperties &memoryProperties) {
	BufferCreateInfo bufferInfo(BufferCreateFlags(), sizeof(vertices[0]) * vertices.size(), BufferUsageFlagBits::eVertexBuffer, SharingMode::eExclusive);
	try {
		buffer = device.createBuffer(bufferInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create vertex buffer!");
	}
	MemoryRequirements memRequirements = device.getBufferMemoryRequirements(buffer);
	uint32_t memType = findMemoryType(memRequirements.memoryTypeBits, MemoryPropertyFlagBits::eHostVisible | MemoryPropertyFlagBits::eHostCoherent, memoryProperties);
	MemoryAllocateInfo allocInfo(memRequirements.size, memType);
	try {
		bufferMemory = device.allocateMemory(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}
	device.bindBufferMemory(buffer, bufferMemory, 0);
	void* data = device.mapMemory(bufferMemory, 0, bufferInfo.size);
	memcpy(data, vertices.data(), (size_t)bufferInfo.size);
	device.unmapMemory(bufferMemory);
}

void GPUVertexBuffer::cleanup(const Device &device) {
	device.destroyBuffer(buffer);
	device.freeMemory(bufferMemory);
}

uint32_t GPUVertexBuffer::findMemoryType(uint32_t typeFilter, MemoryPropertyFlags properties, const PhysicalDeviceMemoryProperties &memoryProperties) {
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("failed to find suitable memory type!");
}
