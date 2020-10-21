#include "Buffer.h"
#include "BufferUtils.h"

namespace vkr {
	void Buffer::createDataBuffer() {
		BufferUtils::createBuffer(deviceManager, size, vk::BufferUsageFlagBits::eTransferSrc, BufferUtils::STANDARD_PROPERTIES, stagingBuffer, stagingBufferMemory);
		BufferUtils::createBuffer(deviceManager, size, vk::BufferUsageFlagBits::eTransferDst | usage, vk::MemoryPropertyFlagBits::eDeviceLocal, buffer, bufferMemory);
	}

	void Buffer::cleanup() {
		device.destroyBuffer(stagingBuffer, nullptr);
		device.freeMemory(stagingBufferMemory, nullptr);
		device.destroyBuffer(buffer, nullptr);
		device.freeMemory(bufferMemory, nullptr);
	}

	Buffer::Buffer(const DeviceManager &deviceManager, const vk::BufferUsageFlags &usage, const uint64_t size): RendererComponent(deviceManager), usage(usage), size(size) {}
}