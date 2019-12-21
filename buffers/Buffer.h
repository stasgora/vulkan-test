#ifndef TEST_BUFFER_H
#define TEST_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	template <class T> class Buffer {
	public:
		Buffer(const std::vector<T> &data, const vk::BufferUsageFlagBits &usage);

		void createDataBuffer(const DeviceManager &deviceManager);
		void cleanup(const vk::Device &device);

		vk::Buffer buffer;
	protected:
		static void createBuffer(const DeviceManager &deviceManager, vk::DeviceSize size, const vk::BufferUsageFlags& usage,
		                         const vk::MemoryPropertyFlags& properties, vk::Buffer &buffer, vk::DeviceMemory &memory);
		static uint32_t findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties, const vk::PhysicalDeviceMemoryProperties &memoryProperties);
		static void copyBuffer(const vk::Buffer &srcBuffer, const vk::Buffer &dstBuffer, vk::DeviceSize size, const DeviceManager &deviceManager);

		vk::DeviceMemory bufferMemory;
		const std::vector<T> &data;
		const vk::BufferUsageFlagBits usage;
	};
}

template class vkr::Buffer<vkr::Vertex>;
template class vkr::Buffer<uint16_t>;

#endif //TEST_BUFFER_H
