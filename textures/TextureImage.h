#ifndef TEST_TEXTUREIMAGE_H
#define TEST_TEXTUREIMAGE_H


#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	class TextureImage {
	public:
		void createTextureImage(const DeviceManager &deviceManager);
		void cleanup(const vk::Device &device);

		static void createImage(const DeviceManager &deviceManager, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
				vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &imageMemory);

	private:
		vk::Image textureImage;
		vk::DeviceMemory textureImageMemory;
	};
}


#endif //TEST_TEXTUREIMAGE_H
