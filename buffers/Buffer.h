#ifndef TEST_BUFFER_H
#define TEST_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"
#include "BufferUtils.h"

namespace vkr {
	template <class T> class Buffer {
	public:
		Buffer(const std::vector<T> &data, const vk::BufferUsageFlagBits &usage);

		void createDataBuffer(const DeviceManager &deviceManager);
		void cleanup(const vk::Device &device);

		vk::Buffer buffer;
	protected:
		vk::DeviceMemory bufferMemory;
		const std::vector<T> &data;
		const vk::BufferUsageFlagBits usage;
	};
}

template class vkr::Buffer<vkr::Vertex>;
template class vkr::Buffer<uint16_t>;

#endif //TEST_BUFFER_H
