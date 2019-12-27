#ifndef TEST_DEPTHIMAGE_H
#define TEST_DEPTHIMAGE_H


#include "TextureImage.h"
#include "../components/RendererComponent.h"

namespace vkr {
	class DepthImage : public BaseImage {
	public:
		explicit DepthImage(const DeviceManager &deviceManager);

		void createDepthResources(const vk::Extent2D &extent);

		static vk::Format findDepthFormat(const DeviceManager &deviceManager);
	private:
		static vk::Format findSupportedFormat(const DeviceManager &deviceManager, const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, const vk::FormatFeatureFlags& features);
		inline bool hasStencilComponent(vk::Format format);
	};
}


#endif //TEST_DEPTHIMAGE_H
