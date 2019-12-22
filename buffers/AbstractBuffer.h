#ifndef TEST_ABSTRACTBUFFER_H
#define TEST_ABSTRACTBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	class AbstractBuffer {
	public:
		static void createBuffer(const DeviceManager &deviceManager, vk::DeviceSize size, const vk::BufferUsageFlags& usage,
		                         const vk::MemoryPropertyFlags& properties, vk::Buffer &buffer, vk::DeviceMemory &memory);

		constexpr static const vk::MemoryPropertyFlags STANDARD_PROPERTIES = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

		static void copyBufferData(const vk::Device &device, vk::DeviceMemory &bufferMemory, const void *data, const size_t size);

	protected:
		static void copyBuffer(const vk::Buffer &srcBuffer, const vk::Buffer &dstBuffer, vk::DeviceSize size, const DeviceManager &deviceManager);
		static uint32_t findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties, const vk::PhysicalDeviceMemoryProperties &memoryProperties);
	};

}


#endif //TEST_ABSTRACTBUFFER_H
