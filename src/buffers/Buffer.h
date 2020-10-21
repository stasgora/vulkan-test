#ifndef TEST_BUFFER_H
#define TEST_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"
#include "BufferUtils.h"
#include "../components/RendererComponent.h"

namespace vkr {
	class Buffer : public RendererComponent {
	public:
		Buffer(const DeviceManager &deviceManager, const vk::BufferUsageFlags &usage, const uint64_t size);

		void createDataBuffer();
		void cleanup() override;

		template<typename T> uint64_t insertData(const std::vector<T> &data) {
			uint64_t dataSize = sizeof(data[0]) * data.size();
			BufferUtils::copyBufferData(device, stagingBufferMemory, data.data(), dataSize, offset);
			BufferUtils::copyBuffer(stagingBuffer, buffer, dataSize, offset, deviceManager);
			uint64_t prevOffset = offset;
			offset += dataSize;
			return prevOffset;
		}

		vk::Buffer buffer;
	protected:
		uint64_t offset = 0;
		const uint64_t size;

		vk::DeviceMemory bufferMemory;
		const vk::BufferUsageFlags usage;

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
	};
}

#endif //TEST_BUFFER_H
