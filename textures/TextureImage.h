#ifndef TEST_TEXTUREIMAGE_H
#define TEST_TEXTUREIMAGE_H


#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"
#include "../components/RendererComponent.h"

namespace vkr {
	class TextureImage : public RendererComponent {
	public:
		TextureImage(const vkr::DeviceManager &deviceManager, const char* path, vk::ImageLayout layout);

		void init() override;
		void cleanup() override;

		vk::ImageView textureImageView;
		vk::ImageLayout layout;
		const char* path;
	private:
		void createTextureImage();
		void createTextureImageView();

		vk::Image textureImage;
		vk::DeviceMemory textureImageMemory;
	};
}


#endif //TEST_TEXTUREIMAGE_H
