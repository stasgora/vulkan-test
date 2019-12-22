#include "Buffer.h"
#include "AbstractBuffer.h"


template<class T> vkr::Buffer<T>::Buffer(const std::vector<T> &data, const vk::BufferUsageFlagBits &usage): data(data), usage(usage) {}

template<class T> void vkr::Buffer<T>::createDataBuffer(const DeviceManager &deviceManager) {
	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	const vk::Device &device = *deviceManager.device;
	vk::DeviceSize bufferSize = sizeof(data[0]) * data.size();
	createBuffer(deviceManager, bufferSize, vk::BufferUsageFlagBits::eTransferSrc, STANDARD_PROPERTIES, stagingBuffer, stagingBufferMemory);
	copyBufferData(device, stagingBufferMemory, data.data(), bufferSize);

	vk::BufferUsageFlags bufferUsage = vk::BufferUsageFlagBits::eTransferDst | usage;
	createBuffer(deviceManager, bufferSize, bufferUsage, vk::MemoryPropertyFlagBits::eDeviceLocal, buffer, bufferMemory);
	copyBuffer(stagingBuffer, buffer, bufferSize, deviceManager);

	device.destroyBuffer(stagingBuffer);
	device.freeMemory(stagingBufferMemory);
}

template<class T> void vkr::Buffer<T>::cleanup(const vk::Device &device) {
	device.destroyBuffer(buffer);
	device.freeMemory(bufferMemory);
}
