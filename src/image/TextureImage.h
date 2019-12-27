#ifndef TEST_TEXTUREIMAGE_H
#define TEST_TEXTUREIMAGE_H


#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"
#include "../components/RendererComponent.h"
#include "BaseImage.h"

namespace vkr {
	class TextureImage : public BaseImage {
	public:
		TextureImage(const vkr::DeviceManager &deviceManager, std::string path, vk::ImageLayout layout);

		void init() override;

		vk::ImageLayout layout;
		std::string path;
	private:
		void createTextureImage();
		void createTextureImageView();

	};
}


#endif //TEST_TEXTUREIMAGE_H
