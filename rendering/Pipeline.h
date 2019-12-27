#ifndef TEST_PIPELINE_H
#define TEST_PIPELINE_H

#include <vulkan/vulkan.hpp>

#include <vector>
#include <string>
#include "../components/RendererComponent.h"
#include "../textures/DepthImage.h"
#include "SwapChain.h"

namespace vkr {
	class Pipeline : public RendererComponent {
	public:
		explicit Pipeline(const DeviceManager &deviceManager);

		void createRenderPass(const vk::Format &format);
		void createGraphicsPipeline(const vk::Extent2D &extent, const vk::DescriptorSetLayout &layout);
		void createFrameBuffers(const SwapChain &swapChain, const DepthImage &depthImage);
		void cleanup() override;

		std::vector<vk::Framebuffer> swapChainFramebuffers;
		vk::RenderPass renderPass;
		vk::Pipeline graphicsPipeline;
		vk::PipelineLayout pipelineLayout;
	private:
		static vk::UniqueShaderModule createShaderModule(const std::vector<char> &code, const vk::Device &device);
		static std::vector<char> readFile(const std::string &fileName);

	};
}

#endif //TEST_PIPELINE_H
