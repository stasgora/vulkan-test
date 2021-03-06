#include "Buffer.h"
#include "BufferUtils.h"


template<class T> void vkr::Buffer<T>::createDataBuffer() {
	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	vk::DeviceSize bufferSize = sizeof(data[0]) * data.size();
	BufferUtils::createBuffer(deviceManager, bufferSize, vk::BufferUsageFlagBits::eTransferSrc, BufferUtils::STANDARD_PROPERTIES, stagingBuffer, stagingBufferMemory);
	BufferUtils::copyBufferData(device, stagingBufferMemory, data.data(), bufferSize);

	vk::BufferUsageFlags bufferUsage = vk::BufferUsageFlagBits::eTransferDst | usage;
	BufferUtils::createBuffer(deviceManager, bufferSize, bufferUsage, vk::MemoryPropertyFlagBits::eDeviceLocal, buffer, bufferMemory);
	BufferUtils::copyBuffer(stagingBuffer, buffer, bufferSize, deviceManager);

	device.destroyBuffer(stagingBuffer, nullptr);
	device.freeMemory(stagingBufferMemory, nullptr);
}

template<class T> void vkr::Buffer<T>::cleanup() {
	device.destroyBuffer(buffer, nullptr);
	device.freeMemory(bufferMemory, nullptr);
}

template<class T> vkr::Buffer<T>::Buffer(const vkr::DeviceManager &deviceManager, const std::vector<T> &data, const vk::BufferUsageFlagBits &usage):
RendererComponent(deviceManager), data(data), usage(usage) {}

template<class T>
size_t vkr::Buffer<T>::getDataSize() const {
	return data.size();
}
