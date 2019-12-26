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
	transitionImageLayout(deviceManager, textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
	copyBufferToImage(deviceManager, stagingBuffer, textureImage, texWidth, texHeight);
	transitionImageLayout(deviceManager, textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

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

void vkr::TextureImage::copyBufferToImage(const vkr::DeviceManager &deviceManager, const vk::Buffer &buffer, const vk::Image &image, uint32_t width, uint32_t height) {
	auto commandBuffer = vkr::AbstractBuffer::createSingleUsageCommandBuffer(*deviceManager.device);
	vk::BufferImageCopy region;
	region.imageSubresource = {vk::ImageAspectFlagBits::eColor, 0, 0, 1};
	vk::Extent3D extent(width, height, 1);
	region.imageExtent = extent;
	commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);
	vkr::AbstractBuffer::endSingleUsageCommandBuffer(deviceManager, commandBuffer);
}

void vkr::TextureImage::transitionImageLayout(const vkr::DeviceManager &deviceManager, const vk::Image &image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destStage;
	auto commandBuffer = vkr::AbstractBuffer::createSingleUsageCommandBuffer(*deviceManager.device);
	vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	vk::ImageMemoryBarrier barrier(vk::AccessFlags(), vk::AccessFlags(), oldLayout, newLayout, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, image, range);

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
		barrier.srcAccessMask = vk::AccessFlags();
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destStage = vk::PipelineStageFlagBits::eTransfer;
	} else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destStage = vk::PipelineStageFlagBits::eFragmentShader;
	} else
		throw std::invalid_argument("unsupported layout transition!");

	commandBuffer.pipelineBarrier(sourceStage, destStage, vk::DependencyFlags(), 0,
			nullptr,0, nullptr, 1, &barrier);
	vkr::AbstractBuffer::endSingleUsageCommandBuffer(deviceManager, commandBuffer);
}

void vkr::TextureImage::cleanup(const vk::Device &device) {
	device.destroyImage(textureImage);
	device.freeMemory(textureImageMemory);
}
