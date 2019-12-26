#ifndef TEST_UNIFORMBUFFER_H
#define TEST_UNIFORMBUFFER_H


#include <vector>
#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"
#include "../components/RendererComponent.h"

namespace vkr {
	class UniformBuffer : public RendererComponent {
	public:
		UniformBuffer(const DeviceManager &deviceManager);

		void createUniformBuffers(uint32_t swapImageCount);
		void updateUniformBuffer(uint32_t imageIndex, const vk::Extent2D &extent);
		void cleanup() override;

		std::vector<vk::Buffer> uniformBuffers;
	private:
		std::vector<vk::DeviceMemory> uniformBufferMemory;
	};
}


#endif //TEST_UNIFORMBUFFER_H
