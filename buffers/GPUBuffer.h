#ifndef TEST_GPUBUFFER_H
#define TEST_GPUBUFFER_H

#include <vulkan/vulkan.hpp>
using namespace vk;
#include "../components/GPUDeviceManager.h"

class GPUBuffer {
public:
	void cleanup(const Device &device);

	Buffer buffer;
protected:
	static uint32_t findMemoryType(uint32_t typeFilter, const MemoryPropertyFlags& properties, const PhysicalDeviceMemoryProperties &memoryProperties);
	static void copyBuffer(const Buffer &srcBuffer, const Buffer &dstBuffer, DeviceSize size, const GPUDeviceManager &deviceManager);

	DeviceMemory bufferMemory;
};


#endif //TEST_GPUBUFFER_H
