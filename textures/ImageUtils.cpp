#include "../buffers/BufferUtils.h"
#include "../libs/stb_image.h"
#include "TextureImage.h"
#include "ImageUtils.h"

vk::ImageView vkr::ImageUtils::createImageView(const vk::Device &device, const vk::Image &image, vk::Format format) {
	vk::ImageViewCreateInfo viewInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, format);
	viewInfo.subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};
	try {
		return device.createImageView(viewInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create texture image view!");
	}
}

void vkr::ImageUtils::createImage(const DeviceManager &deviceManager, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
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
	vk::MemoryAllocateInfo allocInfo(memRequirements.size, BufferUtils::findMemoryType(memRequirements.memoryTypeBits, properties, deviceManager.memoryProperties));
	try {
		imageMemory = device.allocateMemory(allocInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to allocate image memory!");
	}
	device.bindImageMemory(image, imageMemory, 0);
}

void vkr::ImageUtils::copyBufferToImage(const DeviceManager &deviceManager, const vk::Buffer &buffer, const vk::Image &image, uint32_t width, uint32_t height) {
	auto commandBuffer = BufferUtils::createSingleUsageCommandBuffer(*deviceManager.device);
	vk::BufferImageCopy region;
	region.imageSubresource = {vk::ImageAspectFlagBits::eColor, 0, 0, 1};
	vk::Extent3D extent(width, height, 1);
	region.imageExtent = extent;
	commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);
	BufferUtils::endSingleUsageCommandBuffer(deviceManager, commandBuffer);
}

void vkr::ImageUtils::transitionImageLayout(const DeviceManager &deviceManager, const vk::Image &image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destStage;
	auto commandBuffer = BufferUtils::createSingleUsageCommandBuffer(*deviceManager.device);
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
	BufferUtils::endSingleUsageCommandBuffer(deviceManager, commandBuffer);
}