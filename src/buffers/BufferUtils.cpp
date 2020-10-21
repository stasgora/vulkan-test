#include "BufferUtils.h"
#include "CommandBuffer.h"

vk::CommandPool vkr::BufferUtils::singleUsageCommandPool = nullptr;

void vkr::BufferUtils::createBuffer(const DeviceManager &deviceManager, vk::DeviceSize size, const vk::BufferUsageFlags& usage,
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

void vkr::BufferUtils::createSingleUsageCommandPool(const vkr::DeviceManager &deviceManager) {
	vkr::CommandBuffer::createCommandPool(singleUsageCommandPool, vk::CommandPoolCreateFlagBits::eTransient, *deviceManager.device, deviceManager.queueFamilyIndices);
}

void vkr::BufferUtils::copyBuffer(const vk::Buffer &srcBuffer, const vk::Buffer &dstBuffer, vk::DeviceSize size, const DeviceManager &deviceManager) {
	const vk::Device &device = *deviceManager.device;
	vk::CommandBuffer commandBuffer = createSingleUsageCommandBuffer(device);
	vk::BufferCopy copyRegion(0, 0, size);
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);
	endSingleUsageCommandBuffer(deviceManager, commandBuffer);
}

vk::CommandBuffer vkr::BufferUtils::createSingleUsageCommandBuffer(const vk::Device &device) {
	vk::CommandBufferAllocateInfo allocInfo(singleUsageCommandPool, vk::CommandBufferLevel::ePrimary, 1);
	vk::CommandBuffer commandBuffer = device.allocateCommandBuffers(allocInfo)[0];
	vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	commandBuffer.begin(beginInfo);
	return commandBuffer;
}

void vkr::BufferUtils::endSingleUsageCommandBuffer(const vkr::DeviceManager &deviceManager, vk::CommandBuffer &commandBuffer) {
	commandBuffer.end();
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	deviceManager.graphicsQueue.submit(1, &submitInfo, nullptr);
	deviceManager.graphicsQueue.waitIdle();

	deviceManager.device->freeCommandBuffers(singleUsageCommandPool, 1, &commandBuffer);
}

uint32_t vkr::BufferUtils::findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties, const vk::PhysicalDeviceMemoryProperties &memoryProperties) {
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("failed to find suitable memory type!");
}

void vkr::BufferUtils::copyBufferData(const vk::Device &device, vk::DeviceMemory &bufferMemory, const void *data, const size_t size) {
	void* bufferData = device.mapMemory(bufferMemory, 0, size); //TODO use VulkanMemoryAllocator to allocate memory
	memcpy(bufferData, data, size);
	device.unmapMemory(bufferMemory);
}

void vkr::BufferUtils::cleanupSingleUsageCommandPool(const vk::Device &device) {
	device.destroyCommandPool(singleUsageCommandPool, nullptr);
}
