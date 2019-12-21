#ifndef TEST_DESCRIPTORSETLAYOUT_H
#define TEST_DESCRIPTORSETLAYOUT_H


#include <vulkan/vulkan.hpp>

namespace vkr {
	class DescriptorSetLayout {
	public:
		void createDescriptorSetLayout(const vk::Device &device);
		void cleanup(const vk::Device &device);

		vk::DescriptorSetLayout descriptorSetLayout;
	};
}


#endif //TEST_DESCRIPTORSETLAYOUT_H
