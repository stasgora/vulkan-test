#ifndef TEST_GRAPHICSPIPELINE_H
#define TEST_GRAPHICSPIPELINE_H

#include <vulkan/vulkan.hpp>
using namespace vk;

#include <vector>
#include <string>

class GraphicsPipeline {
public:
	void setupPipeline(Device &device, Extent2D &extent, Format &format);
	void cleanup(Device &device);

private:
	void createRenderPass(Device &device, Format &format);
	void createGraphicsPipeline(Device &device, Extent2D &extent);

	static UniqueShaderModule createShaderModule(const std::vector<char> &code, Device &device);
	static std::vector<char> readFile(const std::string &fileName);

	Pipeline graphicsPipeline;
	PipelineLayout pipelineLayout;
	RenderPass renderPass;
};

#endif //TEST_GRAPHICSPIPELINE_H
