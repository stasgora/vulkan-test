#ifndef TEST_UNIFORMBUFFER_H
#define TEST_UNIFORMBUFFER_H


#include <vector>
#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	class UniformBuffer {
	public:
		void createUniformBuffers(const DeviceManager &deviceManager, uint32_t swapImageCount);
		void updateUniformBuffer(const vk::Device &device, uint32_t imageIndex, const vk::Extent2D &extent);
		void cleanup(const vk::Device &device);

		std::vector<vk::Buffer> uniformBuffers;
	private:
		std::vector<vk::DeviceMemory> uniformBufferMemory;
	};
}


#endif //TEST_UNIFORMBUFFER_H
