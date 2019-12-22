#include "DescriptorSet.h"
#include "VulkanStructs.h"

void vkr::DescriptorSet::createDescriptorPool(const vk::Device &device, uint32_t swapImageCount) {
	vk::DescriptorPoolSize poolSize(vk::DescriptorType::eUniformBuffer, swapImageCount);
	vk::DescriptorPoolCreateInfo poolInfo(vk::DescriptorPoolCreateFlags(), swapImageCount, 1, &poolSize);
	try {
		descriptorPool = device.createDescriptorPool(poolInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void vkr::DescriptorSet::createDescriptorSets(const vk::Device &device, uint32_t swapImageCount, const std::vector<vk::Buffer> &uniformBuffers) {
	std::vector<vk::DescriptorSetLayout> layouts(swapImageCount, descriptorSetLayout);
	vk::DescriptorSetAllocateInfo allocInfo(descriptorPool, swapImageCount, layouts.data());
	try {
		descriptorSets = device.allocateDescriptorSets(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create descriptor sets!");
	}
	for (int i = 0; i < swapImageCount; ++i) {
		vk::DescriptorBufferInfo bufferInfo(uniformBuffers[i], 0, sizeof(UniformBufferObject));
		vk::WriteDescriptorSet descriptorWrite(descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer);
		descriptorWrite.pBufferInfo = &bufferInfo;
		device.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
	}
}

void vkr::DescriptorSet::createDescriptorSetLayout(const vk::Device &device) {
	vk::DescriptorSetLayoutBinding uboLayoutBinding(0, vk::DescriptorType::eUniformBuffer,
	                                                1, vk::ShaderStageFlagBits::eVertex, nullptr);
	vk::DescriptorSetLayoutCreateInfo layoutInfo(vk::DescriptorSetLayoutCreateFlags(), 1, &uboLayoutBinding);
	try {
		descriptorSetLayout = device.createDescriptorSetLayout(layoutInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

}

void vkr::DescriptorSet::cleanup(const vk::Device &device) {
	device.destroyDescriptorPool(descriptorPool);
}

void vkr::DescriptorSet::cleanupLayout(const vk::Device &device) {
	device.destroyDescriptorSetLayout(descriptorSetLayout);
}
