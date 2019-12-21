#include "DescriptorSetLayout.h"

void vkr::DescriptorSetLayout::createDescriptorSetLayout(const vk::Device &device) {
	vk::DescriptorSetLayoutBinding uboLayoutBinding(1, vk::DescriptorType::eUniformBuffer,
	                                                1, vk::ShaderStageFlagBits::eVertex, nullptr);
	vk::DescriptorSetLayoutCreateInfo layoutInfo(vk::DescriptorSetLayoutCreateFlags(), 1, &uboLayoutBinding);
	try {
		descriptorSetLayout = device.createDescriptorSetLayout(layoutInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

}

void vkr::DescriptorSetLayout::cleanup(const vk::Device &device) {
	device.destroyDescriptorSetLayout(descriptorSetLayout);
}
