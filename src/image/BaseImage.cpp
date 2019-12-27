#include "ImageUtils.h"
#include "../../libs/stb_image.h"
#include "BaseImage.h"

void vkr::BaseImage::cleanup() {
	device.destroyImageView(imageView);
	device.destroyImage(image);
	device.freeMemory(imageMemory);
}

vkr::BaseImage::BaseImage(const vkr::DeviceManager &deviceManager) : RendererComponent(deviceManager) {}
