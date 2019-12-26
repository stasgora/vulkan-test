#ifndef TEST_SAMPLEDTEXTURE_H
#define TEST_SAMPLEDTEXTURE_H


#include "TextureImage.h"

namespace vkr {
	class SampledTexture: public TextureImage {
	public:
		void init(const DeviceManager &deviceManager, const char *path) override;
		void cleanup(const vk::Device &device) override;

	private:
		void createTextureSampler(const vk::Device &device);

		vk::Sampler textureSampler;
	};
}


#endif //TEST_SAMPLEDTEXTURE_H
