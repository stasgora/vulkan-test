#include "DescriptorSet.h"
#include "VulkanStructs.h"
#include "../textures/SampledTexture.h"

void vkr::DescriptorSet::createDescriptorPool(uint32_t swapImageCount) {
	std::array<vk::DescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0] = {vk::DescriptorType::eUniformBuffer, swapImageCount};
	poolSizes[1] = {vk::DescriptorType::eCombinedImageSampler, swapImageCount};
	vk::DescriptorPoolCreateInfo poolInfo(vk::DescriptorPoolCreateFlags(), swapImageCount, poolSizes.size(), poolSizes.data());
	try {
		descriptorPool = device.createDescriptorPool(poolInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void vkr::DescriptorSet::createDescriptorSets(uint32_t swapImageCount,
		const std::vector<vk::Buffer> &uniformBuffers, const vkr::SampledTexture & texture) {
	std::vector<vk::DescriptorSetLayout> layouts(swapImageCount, descriptorSetLayout);
	vk::DescriptorSetAllocateInfo allocInfo(descriptorPool, swapImageCount, layouts.data());
	try {
		descriptorSets = device.allocateDescriptorSets(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create descriptor sets!");
	}
	for (int i = 0; i < swapImageCount; ++i) {
		vk::DescriptorBufferInfo bufferInfo(uniformBuffers[i], 0, sizeof(UniformBufferObject));
		vk::DescriptorImageInfo imageInfo(texture.textureSampler, texture.textureImageView, texture.layout);
		std::array<vk::WriteDescriptorSet, 2> descriptorWrites = {};
		descriptorWrites[0] = {descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer};
		descriptorWrites[1] = {descriptorSets[i], 1, 0, 1, vk::DescriptorType::eCombinedImageSampler};
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[1].pImageInfo = &imageInfo;
		device.updateDescriptorSets(descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}
}

void vkr::DescriptorSet::createDescriptorSetLayout() {
	vk::DescriptorSetLayoutBinding uboLayoutBinding(0, vk::DescriptorType::eUniformBuffer,
	                                                1, vk::ShaderStageFlagBits::eVertex, nullptr);
	vk::DescriptorSetLayoutBinding samplerLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler,
			1, vk::ShaderStageFlagBits::eFragment, nullptr);
	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
	vk::DescriptorSetLayoutCreateInfo layoutInfo(vk::DescriptorSetLayoutCreateFlags(), bindings.size(), bindings.data());
	try {
		descriptorSetLayout = device.createDescriptorSetLayout(layoutInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

}

void vkr::DescriptorSet::cleanup() {
	device.destroyDescriptorPool(descriptorPool);
}

void vkr::DescriptorSet::cleanupLayout() {
	device.destroyDescriptorSetLayout(descriptorSetLayout);
}

vkr::DescriptorSet::DescriptorSet(const vkr::DeviceManager &deviceManager) : RendererComponent(deviceManager) {}
