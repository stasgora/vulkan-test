#ifndef TEST_SAMPLEDTEXTURE_H
#define TEST_SAMPLEDTEXTURE_H


#include "TextureImage.h"

namespace vkr {
	class SampledTexture: public TextureImage {
	public:
		SampledTexture(const DeviceManager &deviceManager, const char *path, vk::ImageLayout layout);

		void init() override;
		void cleanup() override;

		vk::Sampler textureSampler;
	private:
		void createTextureSampler();
	};
}


#endif //TEST_SAMPLEDTEXTURE_H
