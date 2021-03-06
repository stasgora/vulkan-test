#include "SampledTexture.h"

#include <utility>
#include "BaseImage.h"

void vkr::SampledTexture::init() {
	TextureImage::init();
	createTextureSampler();
}

void vkr::SampledTexture::createTextureSampler() {
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

void vkr::SampledTexture::cleanup() {
	TextureImage::cleanup();
	device.destroySampler(textureSampler, nullptr);
}

vkr::SampledTexture::SampledTexture(const vkr::DeviceManager &deviceManager, std::string path, vk::ImageLayout layout) :
TextureImage(deviceManager, std::move(path), layout) {}
