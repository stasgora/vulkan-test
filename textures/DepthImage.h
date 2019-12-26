#ifndef TEST_DEPTHIMAGE_H
#define TEST_DEPTHIMAGE_H


#include "TextureImage.h"
#include "../components/RendererComponent.h"

namespace vkr {
	class DepthImage : public RendererComponent {
	public:
		DepthImage(const DeviceManager &deviceManager);

		void createDepthResources();

	private:
		vk::Format findSupportedFormat(const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);

		vk::Image depthImage;
		vk::ImageView depthImageView;
		vk::DeviceMemory depthImageMemory;
	};
}


#endif //TEST_DEPTHIMAGE_H
