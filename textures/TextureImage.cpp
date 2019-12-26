#include "TextureImage.h"
#include "../libs/stb_image.h"
#include "../buffers/BufferUtils.h"
#include "ImageUtils.h"


void vkr::TextureImage::init() {
	createTextureImage();
	createTextureImageView();
}

void vkr::TextureImage::createTextureImage() {
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	vk::DeviceSize imageSize = texWidth * texHeight * 4;
	if (!pixels)
		throw std::runtime_error("failed to load texture image!");

	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	BufferUtils::createBuffer(deviceManager, imageSize, vk::BufferUsageFlagBits::eTransferSrc, BufferUtils::STANDARD_PROPERTIES, stagingBuffer, stagingBufferMemory);
	BufferUtils::copyBufferData(device, stagingBufferMemory, pixels, imageSize);
	stbi_image_free(pixels);

	vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled; // TODO check eR8G8B8A8Unorm support
	ImageUtils::createImage(deviceManager, texWidth, texHeight, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal,
	                        usage, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);
	ImageUtils::transitionImageLayout(deviceManager, textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
	ImageUtils::copyBufferToImage(deviceManager, stagingBuffer, textureImage, texWidth, texHeight);
	ImageUtils::transitionImageLayout(deviceManager, textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, layout);

	device.destroyBuffer(stagingBuffer);
	device.freeMemory(stagingBufferMemory);
}

void vkr::TextureImage::createTextureImageView() {
	textureImageView = ImageUtils::createImageView(device, textureImage, vk::Format::eR8G8B8A8Unorm);
}

void vkr::TextureImage::cleanup() {
	device.destroyImageView(textureImageView);
	device.destroyImage(textureImage);
	device.freeMemory(textureImageMemory);
}

vkr::TextureImage::TextureImage(const vkr::DeviceManager &deviceManager, const char* path, vk::ImageLayout layout):
RendererComponent(deviceManager), path(path), layout(layout) {}
