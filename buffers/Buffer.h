#ifndef TEST_BUFFER_H
#define TEST_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"
#include "BufferUtils.h"
#include "../components/RendererComponent.h"

namespace vkr {
	template <class T> class Buffer : public RendererComponent {
	public:
		Buffer(const DeviceManager &deviceManager, const std::vector<T> &data, const vk::BufferUsageFlagBits &usage);

		void createDataBuffer();
		void cleanup() override;

		vk::Buffer buffer;
	protected:
		vk::DeviceMemory bufferMemory;
		const std::vector<T> &data;
		const vk::BufferUsageFlagBits usage;
	};
}

template class vkr::Buffer<vkr::Vertex>;
template class vkr::Buffer<uint32_t>;

#endif //TEST_BUFFER_H
