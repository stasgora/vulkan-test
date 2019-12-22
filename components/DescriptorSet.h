#ifndef TEST_DESCRIPTORSET_H
#define TEST_DESCRIPTORSET_H


#include <vulkan/vulkan.hpp>

namespace vkr {
	class DescriptorSet {
	public:
		void createDescriptorPool(const vk::Device &device, uint32_t swapImageCount);
		void createDescriptorSets(const vk::Device &device, uint32_t swapImageCount, const std::vector<vk::Buffer> &uniformBuffers);
		void createDescriptorSetLayout(const vk::Device &device);
		void cleanup(const vk::Device &device);
		void cleanupLayout(const vk::Device &device);

		vk::DescriptorSetLayout descriptorSetLayout;
		std::vector<vk::DescriptorSet> descriptorSets;
	private:
		vk::DescriptorPool descriptorPool;
	};
}


#endif //TEST_DESCRIPTORSET_H
