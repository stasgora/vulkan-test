#ifndef TEST_TEXTUREIMAGE_H
#define TEST_TEXTUREIMAGE_H


#include <vulkan/vulkan.hpp>
#include "../components/DeviceManager.h"

namespace vkr {
	class TextureImage {
	public:
		void createTextureImage(const DeviceManager &deviceManager);
	};
}


#endif //TEST_TEXTUREIMAGE_H
