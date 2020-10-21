#include "Pipeline.h"
#include "../components/VulkanStructs.h"
#include <fstream>

void vkr::Pipeline::createFrameBuffers(const SwapChain &swapChain, const DepthImage &depthImage) {
	swapChainFramebuffers.resize(swapChain.swapChainImageViews.size());
	for (int i = 0; i < swapChain.swapChainImageViews.size(); ++i) {
		std::array<vk::ImageView, 2> attachments = {swapChain.swapChainImageViews[i], depthImage.imageView};
		vk::FramebufferCreateInfo framebufferInfo(vk::FramebufferCreateFlags(), renderPass,
				attachments.size(), attachments.data(), swapChain.swapChainExtent.width, swapChain.swapChainExtent.height, 1);
		try {
			swapChainFramebuffers[i] = device.createFramebuffer(framebufferInfo);
		} catch (vk::SystemError &err) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void vkr::Pipeline::createRenderPass(const vk::Format &format) {
	vk::AttachmentDescription colorAttachment;
	colorAttachment.format = format;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
	vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

	vk::AttachmentDescription depthAttachment(vk::AttachmentDescriptionFlags(), DepthImage::findDepthFormat(deviceManager), vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eDontCare, vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);
	vk::AttachmentReference depthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
	                                 vk::AccessFlags(), vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);
	std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
	vk::RenderPassCreateInfo renderPassInfo(vk::RenderPassCreateFlags(), attachments.size(), attachments.data(), 1, &subpass, 1, &dependency);
	try {
		renderPass = device.createRenderPass(renderPassInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create render pass!");
	}
}

void vkr::Pipeline::createGraphicsPipeline(const vk::Extent2D &extent, const vk::DescriptorSetLayout &descriptorSetLayout) {
	auto vertexShaderModule = createShaderModule(readFile("shaders/" + shaderFile + ".vert.spv"), device);
	auto fragmentShaderModule = createShaderModule(readFile("shaders/" + shaderFile + ".frag.spv"), device);

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
	rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
	rasterizer.lineWidth = 1.0f;
	vk::PipelineMultisampleStateCreateInfo multisampling;
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	vk::PipelineColorBlendStateCreateInfo colorBlending;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	try {
		pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	vk::PipelineDepthStencilStateCreateInfo depthStencil(vk::PipelineDepthStencilStateCreateFlags(), VK_TRUE, VK_TRUE, vk::CompareOp::eLess);
	depthStencil.maxDepthBounds = 1;
	vk::GraphicsPipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(), 2, shaderStages, &vertexInputInfo, &inputAssembly,
			nullptr, &viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending,
			nullptr, pipelineLayout, renderPass, 0, nullptr, -1);
	try {
		graphicsPipeline = device.createGraphicsPipeline(nullptr, pipelineInfo).value;
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

void vkr::Pipeline::cleanup() {
	for (const auto &buffer : swapChainFramebuffers)
		device.destroyFramebuffer(buffer, nullptr);
	swapChainFramebuffers.clear();
	device.destroyPipeline(graphicsPipeline, nullptr);
	device.destroyPipelineLayout(pipelineLayout, nullptr);
	device.destroyRenderPass(renderPass, nullptr);
}

vkr::Pipeline::Pipeline(const vkr::DeviceManager &deviceManager, const std::string& shaderFile) : RendererComponent(deviceManager), shaderFile(shaderFile) {}
