#ifndef TEST_TEXTUREIMAGE_H
#define TEST_TEXTUREIMAGE_H


#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	class TextureImage {
	public:
		void createTextureImage(const DeviceManager &deviceManager);
		void createTextureImageView(const vk::Device &device);
		void cleanup(const vk::Device &device);

		static vk::ImageView createImageView(const vk::Device &device, const vk::Image &image, vk::Format format);
	private:
		static void createImage(const DeviceManager &deviceManager, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
		                        vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &imageMemory);
		static void transitionImageLayout(const vkr::DeviceManager &deviceManager, const vk::Image &image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		static void copyBufferToImage(const vkr::DeviceManager &deviceManager, const vk::Buffer &buffer, const vk::Image &image, uint32_t width, uint32_t height);

		vk::Image textureImage;
		vk::DeviceMemory textureImageMemory;
		vk::ImageView textureImageView;

	};
}


#endif //TEST_TEXTUREIMAGE_H
