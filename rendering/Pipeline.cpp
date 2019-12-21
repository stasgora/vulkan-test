#include "Pipeline.h"
#include "../components/VulkanStructs.h"
#include <fstream>

void vkr::Pipeline::createFrameBuffers(const vk::Device &device, const vk::Extent2D &extent, const std::vector<vk::ImageView> &imageViews) {
	swapChainFramebuffers.resize(imageViews.size());
	for (int i = 0; i < imageViews.size(); ++i) {
		vk::ImageView attachments[] = {imageViews[i]};
		vk::FramebufferCreateInfo framebufferInfo(vk::FramebufferCreateFlags(), renderPass, 1, attachments, extent.width, extent.height, 1);
		try {
			swapChainFramebuffers[i] = device.createFramebuffer(framebufferInfo);
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void vkr::Pipeline::setupPipeline(const vk::Device &device, const vk::Extent2D &extent, const vk::Format &format) {
	createRenderPass(device, format);
	createGraphicsPipeline(device, extent);
}

void vkr::Pipeline::createRenderPass(const vk::Device &device, const vk::Format &format) {
	vk::AttachmentDescription colorAttachment;
	colorAttachment.format = format;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
	vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
	                                 vk::AccessFlags(), vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);
	vk::RenderPassCreateInfo renderPassInfo(vk::RenderPassCreateFlags(), 1, &colorAttachment, 1, &subpass, 1, &dependency);
	try {
		renderPass = device.createRenderPass(renderPassInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create render pass!");
	}
}

void vkr::Pipeline::createGraphicsPipeline(const vk::Device &device, const vk::Extent2D &extent) {
	auto vertexShaderModule = createShaderModule(readFile("../shaders/vert.spv"), device);
	auto fragmentShaderModule = createShaderModule(readFile("../shaders/frag.spv"), device);

	vk::PipelineShaderStageCreateInfo shaderStages[] = {
			{ vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, *vertexShaderModule, "main" },
			{ vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, *fragmentShaderModule, "main" }
	};
	auto attributeDescriptions = vkr::Vertex::getAttributeDescriptions();
	auto bindingDescriptions = vkr::Vertex::getBindingDescription();
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo(vk::PipelineVertexInputStateCreateFlags(), 1,
			&bindingDescriptions, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data());
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	vk::Viewport viewport(0, 0, extent.width, extent.height, 0, 1);
	vk::Rect2D scissors({0, 0}, extent);
	vk::PipelineViewportStateCreateInfo viewportState(vk::PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scissors);
	vk::PipelineRasterizationStateCreateInfo rasterizer;
	rasterizer.cullMode = vk::CullModeFlagBits::eBack;
	rasterizer.frontFace = vk::FrontFace::eClockwise;
	rasterizer.lineWidth = 1.0f;
	vk::PipelineMultisampleStateCreateInfo multisampling;
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	vk::PipelineColorBlendStateCreateInfo colorBlending;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	try {
		pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	vk::GraphicsPipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(), 2, shaderStages, &vertexInputInfo, &inputAssembly,
			nullptr, &viewportState, &rasterizer, &multisampling, nullptr, &colorBlending,
			nullptr, pipelineLayout, renderPass, 0, nullptr, -1);
	try {
		graphicsPipeline = device.createGraphicsPipeline(nullptr, pipelineInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
}

std::vector<char> vkr::Pipeline::readFile(const std::string &fileName) {
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

vk::UniqueShaderModule vkr::Pipeline::createShaderModule(const std::vector<char> &code, const vk::Device &device) {
	vk::ShaderModuleCreateInfo createInfo(vk::ShaderModuleCreateFlags(), code.size(), reinterpret_cast<const u_int32_t*>(code.data()));
	try {
		return device.createShaderModuleUnique(createInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create shader module!");
	}
}

void vkr::Pipeline::cleanup(const vk::Device &device) {
	for (const auto &buffer : swapChainFramebuffers)
		device.destroyFramebuffer(buffer);
	swapChainFramebuffers.clear();
	device.destroyPipeline(graphicsPipeline);
	device.destroyPipelineLayout(pipelineLayout);
	device.destroyRenderPass(renderPass);
}
