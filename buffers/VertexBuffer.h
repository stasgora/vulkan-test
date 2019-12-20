#ifndef TEST_VERTEXBUFFER_H
#define TEST_VERTEXBUFFER_H

#include "../components/VulkanStructs.h"
#include "../components/GPUDeviceManager.h"
#include "GPUBuffer.h"

const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f}, {0.2f, 0.0f, 0.8f}},
		{{0.5f, -0.5f}, {0.0f, 0.4f, 0.9f}},
		{{0.5f, 0.5f}, {0.2f, 0.8f, 1.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.5f, 0.7f}}
};
const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

class VertexBuffer: public GPUBuffer {
public:
	void createVertexBuffer(const GPUDeviceManager &deviceManager);

private:
	static void createBuffer(const GPUDeviceManager &deviceManager, DeviceSize size, const BufferUsageFlags& usage,
			const MemoryPropertyFlags& properties, Buffer &buffer, DeviceMemory &memory);
};


#endif //TEST_VERTEXBUFFER_H
