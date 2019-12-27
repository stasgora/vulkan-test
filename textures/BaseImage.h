#ifndef TEST_BASEIMAGE_H
#define TEST_BASEIMAGE_H


#include <vulkan/vulkan.hpp>
#include "../components/RendererComponent.h"
#include "../components/DeviceManager.h"

namespace vkr {
	class BaseImage : public RendererComponent {
	public:
		explicit BaseImage(const DeviceManager &deviceManager);

		void cleanup() override;

		vk::ImageView imageView;

	protected:
		vk::Image image;
		vk::DeviceMemory imageMemory;
	};
}


#endif //TEST_BASEIMAGE_H
