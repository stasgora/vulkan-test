#include "GraphicsPipeline.h"
#include <fstream>

void GraphicsPipeline::createGraphicsPipeline(Device &device, Extent2D &extent) {
	auto vertexShaderModule = createShaderModule(readFile("shaders/vert.spv"), device);
	auto fragmentShaderModule = createShaderModule(readFile("shaders/frag.spv"), device);

	PipelineShaderStageCreateInfo shaderStages[] = {
			{ PipelineShaderStageCreateFlags(), ShaderStageFlagBits::eVertex, *vertexShaderModule, "main" },
			{ PipelineShaderStageCreateFlags(), ShaderStageFlagBits::eFragment, *fragmentShaderModule, "main" }
	};
	PipelineVertexInputStateCreateInfo vertexInputInfo;
	PipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	Viewport viewport(0, 0, extent.width, extent.height, 0, 1);
	Rect2D scissors({0, 0}, extent);
	PipelineViewportStateCreateInfo viewportState(PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scissors);


}

std::vector<char> GraphicsPipeline::readFile(const std::string &fileName) {
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error("failed to open file " + fileName);
	size_t fileSize = file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

UniqueShaderModule GraphicsPipeline::createShaderModule(const std::vector<char> &code, Device &device) {
	ShaderModuleCreateInfo createInfo(ShaderModuleCreateFlags(), code.size(), reinterpret_cast<const u_int32_t*>(code.data()));
	try {
		return device.createShaderModuleUnique(createInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create shader module!");
	}
}
