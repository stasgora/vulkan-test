#include "UniformBuffer.h"
#include "../components/VulkanStructs.h"
#include "AbstractBuffer.h"

void vkr::UniformBuffer::createUniformBuffers(const DeviceManager &deviceManager, uint32_t swapImageCount) {
	vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
	uniformBuffers.resize(swapImageCount);
	uniformBufferMemory.resize(swapImageCount);
	for (int i = 0; i < swapImageCount; ++i) {
		AbstractBuffer::createBuffer(deviceManager, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
				AbstractBuffer::STANDARD_PROPERTIES, uniformBuffers[i], uniformBufferMemory[i]);
	}
}

void vkr::UniformBuffer::updateUniformBuffer(uint32_t imageIndex) {

}

void vkr::UniformBuffer::cleanup(const vk::Device &device, uint32_t swapImageCount) {
	for (int i = 0; i < swapImageCount; ++i) {
		device.destroyBuffer(uniformBuffers[i]);
		device.freeMemory(uniformBufferMemory[i]);
	}
}
