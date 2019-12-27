#ifndef TEST_IMAGEUTILS_H
#define TEST_IMAGEUTILS_H


#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	class ImageUtils {
	public:
		static vk::ImageView createImageView(const vk::Device &device, const vk::Image &image, vk::Format format, vk::ImageAspectFlags aspectFlags);

		static void copyBufferToImage(const DeviceManager &deviceManager, const vk::Buffer &buffer, const vk::Image &image, uint32_t width, uint32_t height);

		static void createImage(const DeviceManager &deviceManager, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
		                        vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &imageMemory);

		static void transitionImageLayout(const DeviceManager &deviceManager, const vk::Image &image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
	};
}


#endif //TEST_IMAGEUTILS_H
