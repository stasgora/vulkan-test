#include "GraphicsRenderer.h"
#include "../components/GPUDeviceManager.h"

void GraphicsRenderer::setupRendering(Device &device) {
	try {
		imageAvailableSemaphore = device.createSemaphore({});
		renderFinishedSemaphore = device.createSemaphore({});
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
}

void GraphicsRenderer::drawFrame(Device &device, SwapChain &swapChain, std::vector<vk::CommandBuffer, std::allocator<vk::CommandBuffer>> &buffers,
                                 GPUDeviceManager &deviceManager) {
	uint32_t imageIndex;
	device.acquireNextImageKHR(swapChain.swapChain, UINT32_MAX, imageAvailableSemaphore, nullptr, &imageIndex);
	Semaphore waitSemaphores[] = {imageAvailableSemaphore};
	PipelineStageFlags waitStages[] = {PipelineStageFlagBits::eColorAttachmentOutput};
	Semaphore signalSemaphore[] = {renderFinishedSemaphore};
	SubmitInfo submitInfo(1, waitSemaphores, waitStages, 1, &buffers[imageIndex], 1, signalSemaphore);
	try {
		deviceManager.graphicsQueue.submit(submitInfo, nullptr);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}
	SwapchainKHR swapChains[] = {swapChain.swapChain};
	PresentInfoKHR presentInfo(1, signalSemaphore, 1, swapChains, &imageIndex);
	deviceManager.presentQueue.presentKHR(presentInfo);
}

void GraphicsRenderer::cleanup(Device &device) {
	device.destroySemaphore(imageAvailableSemaphore);
	device.destroySemaphore(renderFinishedSemaphore);
}
