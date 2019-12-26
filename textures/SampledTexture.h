#ifndef TEST_SAMPLEDTEXTURE_H
#define TEST_SAMPLEDTEXTURE_H


#include "TextureImage.h"

namespace vkr {
	class SampledTexture: public TextureImage {
	public:
		SampledTexture(const char *path, vk::ImageLayout layout);

		void init(const DeviceManager &deviceManager) override;
		void cleanup(const vk::Device &device) override;

		vk::Sampler textureSampler;
	private:
		void createTextureSampler(const vk::Device &device);
	};
}


#endif //TEST_SAMPLEDTEXTURE_H
