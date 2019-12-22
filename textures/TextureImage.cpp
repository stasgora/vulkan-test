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

	device.destroyBuffer(stagingBuffer);
	device.freeMemory(stagingBufferMemory);
}
