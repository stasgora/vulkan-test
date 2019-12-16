#include "GraphicsRenderer.h"
#include "../components/GPUDeviceManager.h"

void GraphicsRenderer::setupRendering(Device &device, uint32_t swapImageCount) {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapImageCount, -1);
	try {
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
			imageAvailableSemaphores[i] = device.createSemaphore({});
			renderFinishedSemaphores[i] = device.createSemaphore({});
			inFlightFences[i] = device.createFence(FenceCreateInfo(FenceCreateFlagBits::eSignaled));
		}
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
}

void GraphicsRenderer::drawFrame(Device &device, SwapChain &swapChain, std::vector<vk::CommandBuffer,
		std::allocator<vk::CommandBuffer>> &buffers, GPUDeviceManager &deviceManager) {
	device.waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	Result result = device.acquireNextImageKHR(swapChain.swapChain, UINT32_MAX, imageAvailableSemaphores[currentFrame], nullptr, &imageIndex);
	if(result == Result::eErrorOutOfDateKHR) {

		return;
	} else if (result != Result::eSuccess && result != Result::eSuboptimalKHR)
		throw std::runtime_error("failed to acquire swap chain image!");
	if (imagesInFlight[imageIndex] >= 0)
		device.waitForFences(1, &inFlightFences[imagesInFlight[imageIndex]], VK_TRUE, UINT64_MAX);
	imagesInFlight[imageIndex] = currentFrame;
	device.resetFences(1, &inFlightFences[currentFrame]);

	Semaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	PipelineStageFlags waitStages[] = {PipelineStageFlagBits::eColorAttachmentOutput};
	Semaphore signalSemaphore[] = {renderFinishedSemaphores[currentFrame]};
	SubmitInfo submitInfo(1, waitSemaphores, waitStages, 1, &buffers[imageIndex], 1, signalSemaphore);
	try {
		deviceManager.graphicsQueue.submit(submitInfo, inFlightFences[currentFrame]);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}
	SwapchainKHR swapChains[] = {swapChain.swapChain};
	PresentInfoKHR presentInfo(1, signalSemaphore, 1, swapChains, &imageIndex);
	deviceManager.presentQueue.presentKHR(presentInfo);
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void GraphicsRenderer::cleanup(Device &device) {
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		device.destroySemaphore(imageAvailableSemaphores[i]);
		device.destroySemaphore(renderFinishedSemaphores[i]);
		device.destroyFence(inFlightFences[i]);
	}
}
