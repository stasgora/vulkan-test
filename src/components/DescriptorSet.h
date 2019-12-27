#ifndef TEST_DESCRIPTORSET_H
#define TEST_DESCRIPTORSET_H


#include <vulkan/vulkan.hpp>
#include "../image/SampledTexture.h"

namespace vkr {
	class DescriptorSet : public RendererComponent {
	public:
		explicit DescriptorSet(const DeviceManager &deviceManager);

		void createDescriptorPool(uint32_t swapImageCount);
		void createDescriptorSets(uint32_t swapImageCount, const std::vector<vk::Buffer> &uniformBuffers, const vkr::SampledTexture &texture);
		void createDescriptorSetLayout();
		void cleanup() override;
		void cleanupLayout();

		vk::DescriptorSetLayout descriptorSetLayout;
		std::vector<vk::DescriptorSet> descriptorSets;
	private:
		vk::DescriptorPool descriptorPool;
	};
}


#endif //TEST_DESCRIPTORSET_H
