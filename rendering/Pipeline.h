#ifndef TEST_PIPELINE_H
#define TEST_PIPELINE_H

#include <vulkan/vulkan.hpp>

#include <vector>
#include <string>

namespace vkr {
	class Pipeline {
	public:
		void createRenderPass(const vk::Device &device, const vk::Format &format);
		void createGraphicsPipeline(const vk::Device &device, const vk::Extent2D &extent, const vk::DescriptorSetLayout &layout);
		void createFrameBuffers(const vk::Device &device, const vk::Extent2D &extent, const std::vector<vk::ImageView> &imageViews);
		void cleanup(const vk::Device &device);

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
