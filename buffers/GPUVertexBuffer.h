#ifndef TEST_GPUVERTEXBUFFER_H
#define TEST_GPUVERTEXBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/VulkanStructs.h"

using namespace vk;

const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

class GPUVertexBuffer {
public:
	void createVertexBuffer(const Device &device, const PhysicalDeviceMemoryProperties &memoryProperties);
	void cleanup(const Device &device);

private:
	uint32_t findMemoryType(uint32_t typeFilter, MemoryPropertyFlags properties, const PhysicalDeviceMemoryProperties &memoryProperties);

	Buffer buffer;
	DeviceMemory bufferMemory;
};


#endif //TEST_GPUVERTEXBUFFER_H
