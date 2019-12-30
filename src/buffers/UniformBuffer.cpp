#include "UniformBuffer.h"
#include "../components/VulkanStructs.h"
#include "BufferUtils.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

void vkr::UniformBuffer::createUniformBuffers(uint32_t swapImageCount) {
	vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
	uniformBuffers.resize(swapImageCount);
	uniformBufferMemory.resize(swapImageCount);
	for (int i = 0; i < swapImageCount; ++i) {
		BufferUtils::createBuffer(deviceManager, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
		                          BufferUtils::STANDARD_PROPERTIES, uniformBuffers[i], uniformBufferMemory[i]);
	}
}

void vkr::UniformBuffer::updateUniformBuffer(uint32_t imageIndex, const vk::Extent2D &extent) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {}; // TODO Use push constants to pass the matrices instead
	ubo.model = glm::rotate(glm::mat4(1), time * glm::radians(45.0f), glm::vec3(0, 0, 1));
	ubo.view = glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	ubo.proj = glm::perspective(glm::radians(45.0f), extent.width / (float) extent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	BufferUtils::copyBufferData(device, uniformBufferMemory[imageIndex], &ubo, sizeof(ubo));
}

void vkr::UniformBuffer::cleanup() {
	for (int i = 0; i < uniformBuffers.size(); ++i) {
		device.destroyBuffer(uniformBuffers[i]);
		device.freeMemory(uniformBufferMemory[i]);
	}
}

vkr::UniformBuffer::UniformBuffer(const vkr::DeviceManager &deviceManager) : RendererComponent(deviceManager) {}
