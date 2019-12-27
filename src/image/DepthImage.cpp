#include "DepthImage.h"
#include "ImageUtils.h"

void vkr::DepthImage::createDepthResources(const vk::Extent2D &extent) {
	auto depthFormat = findDepthFormat(deviceManager);
	ImageUtils::createImage(deviceManager, extent.width, extent.height, depthFormat, vk::ImageTiling::eOptimal,
	                        vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, image, imageMemory);
	imageView = ImageUtils::createImageView(device, image, depthFormat, vk::ImageAspectFlagBits::eDepth);

}

vk::Format vkr::DepthImage::findSupportedFormat(const DeviceManager &deviceManager, const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, const vk::FormatFeatureFlags& features) {
	for (const auto &format : candidates) {
		auto props = deviceManager.physicalDevice.getFormatProperties(format);
		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
			return format;
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
			return format;
	}
	throw std::runtime_error("failed to find supported format");
}

vkr::DepthImage::DepthImage(const vkr::DeviceManager &deviceManager) : BaseImage(deviceManager) {}

vk::Format vkr::DepthImage::findDepthFormat(const DeviceManager &deviceManager) {
	return findSupportedFormat(deviceManager, {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
			vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

bool vkr::DepthImage::hasStencilComponent(vk::Format format) {
	return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD32SfloatS8Uint;
}
