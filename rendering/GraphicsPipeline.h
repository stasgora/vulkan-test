#ifndef TEST_GRAPHICSPIPELINE_H
#define TEST_GRAPHICSPIPELINE_H

#include <vulkan/vulkan.hpp>
using namespace vk;

#include <vector>
#include <string>

class GraphicsPipeline {
public:
	void setupPipeline(const Device &device, const Extent2D &extent, const Format &format);
	void createFrameBuffers(const Device &device, const Extent2D &extent, const std::vector<ImageView> &imageViews);
	void cleanup(const Device &device);

	std::vector<Framebuffer> swapChainFramebuffers;
	RenderPass renderPass;
	Pipeline graphicsPipeline;
private:
	void createRenderPass(const Device &device, const Format &format);
	void createGraphicsPipeline(const Device &device, const Extent2D &extent);

	static UniqueShaderModule createShaderModule(const std::vector<char> &code, const Device &device);
	static std::vector<char> readFile(const std::string &fileName);

	PipelineLayout pipelineLayout;
};

#endif //TEST_GRAPHICSPIPELINE_H
