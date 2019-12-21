#ifndef TEST_GPUBUFFER_H
#define TEST_GPUBUFFER_H

#include <vulkan/vulkan.hpp>
using namespace vk;
#include "../components/GPUDeviceManager.h"

namespace vkr {
	template <class T> class GPUBuffer {
	public:
		GPUBuffer(const std::vector<T> &data, const BufferUsageFlagBits &usage);

		void createDataBuffer(const GPUDeviceManager &deviceManager);
		void cleanup(const Device &device);

		Buffer buffer;
	protected:
		static void createBuffer(const GPUDeviceManager &deviceManager, DeviceSize size, const BufferUsageFlags& usage,
		                         const MemoryPropertyFlags& properties, Buffer &buffer, DeviceMemory &memory);
		static uint32_t findMemoryType(uint32_t typeFilter, const MemoryPropertyFlags& properties, const PhysicalDeviceMemoryProperties &memoryProperties);
		static void copyBuffer(const Buffer &srcBuffer, const Buffer &dstBuffer, DeviceSize size, const GPUDeviceManager &deviceManager);

		DeviceMemory bufferMemory;
		const std::vector<T> data;
		const BufferUsageFlagBits usage;
	};
}

template class vkr::GPUBuffer<vkr::Vertex>;
template class vkr::GPUBuffer<uint16_t>;

#endif //TEST_GPUBUFFER_H
