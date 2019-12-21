#include "GPUBuffer.h"
#include "GPUCommandBuffer.h"


template<class T> void vkr::GPUBuffer<T>::createDataBuffer(const GPUDeviceManager &deviceManager) {
	Buffer stagingBuffer;
	DeviceMemory stagingBufferMemory;
	const Device &device = *deviceManager.device;
	DeviceSize bufferSize = sizeof(data[0]) * data.size();
	MemoryPropertyFlags properties = MemoryPropertyFlagBits::eHostVisible | MemoryPropertyFlagBits::eHostCoherent;
	createBuffer(deviceManager, bufferSize, BufferUsageFlagBits::eTransferSrc, properties, stagingBuffer, stagingBufferMemory);
	void* bufferData = device.mapMemory(stagingBufferMemory, 0, bufferSize); //TODO use VulkanMemoryAllocator to allocate memory
	memcpy(bufferData, data.data(), (size_t) bufferSize);
	device.unmapMemory(stagingBufferMemory);

	BufferUsageFlags bufferUsage = BufferUsageFlagBits::eTransferDst | usage;
	createBuffer(deviceManager, bufferSize, bufferUsage, MemoryPropertyFlagBits::eDeviceLocal, buffer, bufferMemory);
	copyBuffer(stagingBuffer, buffer, bufferSize, deviceManager);

	device.destroyBuffer(stagingBuffer);
	device.freeMemory(stagingBufferMemory);
}

template<class T> void vkr::GPUBuffer<T>::createBuffer(const GPUDeviceManager &deviceManager, DeviceSize size, const BufferUsageFlags& usage,
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

template<class T> void vkr::GPUBuffer<T>::copyBuffer(const Buffer &srcBuffer, const Buffer &dstBuffer, DeviceSize size, const GPUDeviceManager &deviceManager) {
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

template<class T> uint32_t vkr::GPUBuffer<T>::findMemoryType(uint32_t typeFilter, const MemoryPropertyFlags& properties, const PhysicalDeviceMemoryProperties &memoryProperties) {
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("failed to find suitable memory type!");
}

template<class T> void vkr::GPUBuffer<T>::cleanup(const Device &device) {
	device.destroyBuffer(buffer);
	device.freeMemory(bufferMemory);
}

template<class T> vkr::GPUBuffer<T>::GPUBuffer(const std::vector<T> &data, const BufferUsageFlagBits &usage):data(data), usage(usage) {}
