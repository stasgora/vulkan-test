#include "TextureImage.h"
#include "../libs/stb_image.h"
#include "../buffers/AbstractBuffer.h"

void vkr::TextureImage::createTextureImage(const DeviceManager &deviceManager) {
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load("../assets/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	vk::DeviceSize imageSize = texWidth * texHeight * 4;
	if (!pixels)
		throw std::runtime_error("failed to load texture image!");

	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	const vk::Device &device = *deviceManager.device;
	AbstractBuffer::createBuffer(deviceManager, imageSize, vk::BufferUsageFlagBits::eTransferSrc, AbstractBuffer::STANDARD_PROPERTIES, stagingBuffer, stagingBufferMemory);
	AbstractBuffer::copyBufferData(device, stagingBufferMemory, pixels, imageSize);
	stbi_image_free(pixels);

	vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled; // TODO check eR8G8B8A8Unorm support
	createImage(deviceManager, texWidth, texHeight, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal,
			usage, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);

	device.destroyBuffer(stagingBuffer);
	device.freeMemory(stagingBufferMemory);
}

void vkr::TextureImage::createImage(const vkr::DeviceManager &deviceManager, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
                                    vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &imageMemory) {
	vk::Extent3D imageExtent(static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1);
	vk::ImageCreateInfo imageInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, format,
			imageExtent, 1, 1, vk::SampleCountFlagBits::e1, tiling, usage);

	const vk::Device &device = *deviceManager.device;
	try {
		image = device.createImage(imageInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create image!");
	}
	auto memRequirements = device.getImageMemoryRequirements(image);
	vk::MemoryAllocateInfo allocInfo(memRequirements.size, AbstractBuffer::findMemoryType(memRequirements.memoryTypeBits, properties, deviceManager.memoryProperties));
	try {
		imageMemory = device.allocateMemory(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to allocate image memory!");
	}
	device.bindImageMemory(image, imageMemory, 0);
}

void vkr::TextureImage::cleanup(const vk::Device &device) {
	device.destroyImage(textureImage);
	device.freeMemory(textureImageMemory);
}
