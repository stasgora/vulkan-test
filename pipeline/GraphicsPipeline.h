#ifndef TEST_GRAPHICSPIPELINE_H
#define TEST_GRAPHICSPIPELINE_H

#include <vulkan/vulkan.hpp>
using namespace vk;

#include <vector>
#include <string>

class GraphicsPipeline {
public:
	void createGraphicsPipeline(Device &device, Extent2D &extent);

	static UniqueShaderModule createShaderModule(const std::vector<char> &code, Device &device);
	static std::vector<char> readFile(const std::string &fileName);
};

#endif //TEST_GRAPHICSPIPELINE_H
