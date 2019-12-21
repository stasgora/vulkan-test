#include "GraphicsPipeline.h"
#include "../components/VulkanStructs.h"
#include <fstream>

void vkr::GraphicsPipeline::createFrameBuffers(const Device &device, const Extent2D &extent, const std::vector<ImageView> &imageViews) {
	swapChainFramebuffers.resize(imageViews.size());
	for (int i = 0; i < imageViews.size(); ++i) {
		ImageView attachments[] = {imageViews[i]};
		FramebufferCreateInfo framebufferInfo(FramebufferCreateFlags(), renderPass, 1, attachments, extent.width, extent.height, 1);
		try {
			swapChainFramebuffers[i] = device.createFramebuffer(framebufferInfo);
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void vkr::GraphicsPipeline::setupPipeline(const Device &device, const Extent2D &extent, const Format &format) {
	createRenderPass(device, format);
	createGraphicsPipeline(device, extent);
}

void vkr::GraphicsPipeline::createRenderPass(const Device &device, const Format &format) {
	AttachmentDescription colorAttachment;
	colorAttachment.format = format;
	colorAttachment.loadOp = AttachmentLoadOp::eClear;
	colorAttachment.stencilLoadOp = AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = AttachmentStoreOp::eDontCare;
	colorAttachment.finalLayout = ImageLayout::ePresentSrcKHR;
	AttachmentReference colorAttachmentRef(0, ImageLayout::eColorAttachmentOptimal);
	SubpassDescription subpass;
	subpass.pipelineBindPoint = PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0, PipelineStageFlagBits::eColorAttachmentOutput, PipelineStageFlagBits::eColorAttachmentOutput,
	                             AccessFlags(), AccessFlagBits::eColorAttachmentRead | AccessFlagBits::eColorAttachmentWrite);
	RenderPassCreateInfo renderPassInfo(RenderPassCreateFlags(), 1, &colorAttachment, 1, &subpass, 1, &dependency);
	try {
		renderPass = device.createRenderPass(renderPassInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create render pass!");
	}
}

void vkr::GraphicsPipeline::createGraphicsPipeline(const Device &device, const Extent2D &extent) {
	auto vertexShaderModule = createShaderModule(readFile("../shaders/vert.spv"), device);
	auto fragmentShaderModule = createShaderModule(readFile("../shaders/frag.spv"), device);

	PipelineShaderStageCreateInfo shaderStages[] = {
			{ PipelineShaderStageCreateFlags(), ShaderStageFlagBits::eVertex, *vertexShaderModule, "main" },
			{ PipelineShaderStageCreateFlags(), ShaderStageFlagBits::eFragment, *fragmentShaderModule, "main" }
	};
	auto attributeDescriptions = vkr::Vertex::getAttributeDescriptions();
	auto bindingDescriptions = vkr::Vertex::getBindingDescription();
	PipelineVertexInputStateCreateInfo vertexInputInfo(PipelineVertexInputStateCreateFlags(), 1,
			&bindingDescriptions, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data());
	PipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	Viewport viewport(0, 0, extent.width, extent.height, 0, 1);
	Rect2D scissors({0, 0}, extent);
	PipelineViewportStateCreateInfo viewportState(PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scissors);
	PipelineRasterizationStateCreateInfo rasterizer;
	rasterizer.cullMode = CullModeFlagBits::eBack;
	rasterizer.frontFace = FrontFace::eClockwise;
	rasterizer.lineWidth = 1.0f;
	PipelineMultisampleStateCreateInfo multisampling;
	PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = ColorComponentFlagBits::eR | ColorComponentFlagBits::eG | ColorComponentFlagBits::eB | ColorComponentFlagBits::eA;
	PipelineColorBlendStateCreateInfo colorBlending;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	PipelineLayoutCreateInfo pipelineLayoutInfo;
	try {
		pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	GraphicsPipelineCreateInfo pipelineInfo(PipelineCreateFlags(), 2, shaderStages, &vertexInputInfo, &inputAssembly,
			nullptr, &viewportState, &rasterizer, &multisampling, nullptr, &colorBlending,
			nullptr, pipelineLayout, renderPass, 0, nullptr, -1);
	try {
		graphicsPipeline = device.createGraphicsPipeline(nullptr, pipelineInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
}

std::vector<char> vkr::GraphicsPipeline::readFile(const std::string &fileName) {
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

UniqueShaderModule vkr::GraphicsPipeline::createShaderModule(const std::vector<char> &code, const Device &device) {
	ShaderModuleCreateInfo createInfo(ShaderModuleCreateFlags(), code.size(), reinterpret_cast<const u_int32_t*>(code.data()));
	try {
		return device.createShaderModuleUnique(createInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create shader module!");
	}
}

void vkr::GraphicsPipeline::cleanup(const Device &device) {
	for (const auto &buffer : swapChainFramebuffers)
		device.destroyFramebuffer(buffer);
	swapChainFramebuffers.clear();
	device.destroyPipeline(graphicsPipeline);
	device.destroyPipelineLayout(pipelineLayout);
	device.destroyRenderPass(renderPass);
}
