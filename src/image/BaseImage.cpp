#include "ImageUtils.h"
#include "../../libs/stb_image.h"
#include "BaseImage.h"

void vkr::BaseImage::cleanup() {
	device.destroyImageView(imageView, nullptr);
	device.destroyImage(image, nullptr);
	device.freeMemory(imageMemory, nullptr);
}

vkr::BaseImage::BaseImage(const vkr::DeviceManager &deviceManager) : RendererComponent(deviceManager) {}
