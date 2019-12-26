#include "SampledTexture.h"

void vkr::SampledTexture::init(const vkr::DeviceManager &deviceManager, const char *path) {
	TextureImage::init(deviceManager, path);
	createTextureSampler(*deviceManager.device);
}

void vkr::SampledTexture::createTextureSampler(const vk::Device &device) {
	vk::SamplerCreateInfo samplerInfo(vk::SamplerCreateFlags(), vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear);
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerInfo.compareOp = vk::CompareOp::eAlways;
	try {
		textureSampler = device.createSampler(samplerInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void vkr::SampledTexture::cleanup(const vk::Device &device) {
	TextureImage::cleanup(device);
	device.destroySampler(textureSampler);
}
