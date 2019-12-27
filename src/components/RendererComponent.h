#ifndef TEST_RENDERERCOMPONENT_H
#define TEST_RENDERERCOMPONENT_H


#include "DeviceManager.h"

namespace vkr {
	class RendererComponent {
	public:
		explicit RendererComponent(const DeviceManager &deviceManager);
		virtual void init();
		virtual void cleanup();

	protected:
		const DeviceManager &deviceManager;
		const vk::Device &device;
	};
}

#endif //TEST_RENDERERCOMPONENT_H
