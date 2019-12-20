#ifndef TEST_GPUVERTEXBUFFER_H
#define TEST_GPUVERTEXBUFFER_H

#include <vulkan/vulkan.hpp>
#include "../components/VulkanStructs.h"
#include "../components/GPUDeviceManager.h"

using namespace vk;

const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f}, {0.2f, 0.0f, 0.8f}},
		{{0.5f, 0.5f}, {0.0f, 0.4f, 0.9f}},
		{{-0.5f, 0.5f}, {0.2f, 0.8f, 1.0f}}
};

class GPUVertexBuffer {
public:
	void createVertexBuffer(const GPUDeviceManager &deviceManager);
	void cleanup(const Device &device);

	Buffer vertexBuffer;
private:
	static void createBuffer(const GPUDeviceManager &deviceManager, DeviceSize size, const BufferUsageFlags& usage,
			const MemoryPropertyFlags& properties, Buffer &buffer, DeviceMemory &memory);
	static uint32_t findMemoryType(uint32_t typeFilter, const MemoryPropertyFlags& properties, const PhysicalDeviceMemoryProperties &memoryProperties);
	static void copyBuffer(const Buffer &srcBuffer, const Buffer &dstBuffer, DeviceSize size, const GPUDeviceManager &deviceManager);

	DeviceMemory vertexBufferMemory;
};


#endif //TEST_GPUVERTEXBUFFER_H
