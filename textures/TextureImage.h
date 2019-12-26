#ifndef TEST_TEXTUREIMAGE_H
#define TEST_TEXTUREIMAGE_H


#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	class TextureImage {
	public:
		TextureImage(const char* path, vk::ImageLayout layout);

		virtual void init(const DeviceManager &deviceManager);
		virtual void cleanup(const vk::Device &device);

		vk::ImageView textureImageView;
		vk::ImageLayout layout;
		const char* path;
	private:
		void createTextureImage(const DeviceManager &deviceManager);
		void createTextureImageView(const vk::Device &device);

		vk::Image textureImage;
		vk::DeviceMemory textureImageMemory;
	};
}


#endif //TEST_TEXTUREIMAGE_H
