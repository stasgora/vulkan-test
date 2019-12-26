#ifndef TEST_BUFFERUTILS_H
#define TEST_BUFFERUTILS_H

#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	class BufferUtils {
	public:
		static void createSingleUsageCommandPool(const DeviceManager &deviceManager);
		static void cleanupSingleUsageCommandPool(const vk::Device &device);

		static void createBuffer(const DeviceManager &deviceManager, vk::DeviceSize size, const vk::BufferUsageFlags& usage,
		                         const vk::MemoryPropertyFlags& properties, vk::Buffer &buffer, vk::DeviceMemory &memory);
		static vk::CommandBuffer createSingleUsageCommandBuffer(const vk::Device &device);
		static void endSingleUsageCommandBuffer(const vkr::DeviceManager &deviceManager, vk::CommandBuffer &commandBuffer);

		constexpr static const vk::MemoryPropertyFlags STANDARD_PROPERTIES = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

		static void copyBufferData(const vk::Device &device, vk::DeviceMemory &bufferMemory, const void *data, const size_t size);
		static void copyBuffer(const vk::Buffer &srcBuffer, const vk::Buffer &dstBuffer, vk::DeviceSize size, const DeviceManager &deviceManager);
		static uint32_t findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& properties, const vk::PhysicalDeviceMemoryProperties &memoryProperties);

	protected:
		static vk::CommandPool singleUsageCommandPool;
	};

}


#endif //TEST_BUFFERUTILS_H
