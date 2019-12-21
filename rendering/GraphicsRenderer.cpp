#include "GraphicsRenderer.h"
#include "../components/GPUDeviceManager.h"

void vkr::GraphicsRenderer::setupRendering(const Device &device, uint32_t swapImageCount) {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapImageCount, nullptr);
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

bool vkr::GraphicsRenderer::drawFrame(const Device &device, const vkr::SwapChain &swapChain, std::vector<vk::CommandBuffer,
		std::allocator<vk::CommandBuffer>> &buffers, const vkr::GPUDeviceManager &deviceManager) {
	device.waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapChain.swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		return false;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	} //TODO what if suboptimal?

	if (imagesInFlight[imageIndex] != nullptr)
		device.waitForFences(1, imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	imagesInFlight[imageIndex] = &inFlightFences[currentFrame];

	Semaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	PipelineStageFlags waitStages[] = {PipelineStageFlagBits::eColorAttachmentOutput};
	Semaphore signalSemaphore[] = {renderFinishedSemaphores[currentFrame]};
	SubmitInfo submitInfo(1, waitSemaphores, waitStages, 1, &buffers[imageIndex], 1, signalSemaphore);

	device.resetFences(1, &inFlightFences[currentFrame]);
	try {
		deviceManager.graphicsQueue.submit(submitInfo, inFlightFences[currentFrame]);
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}
	SwapchainKHR swapChains[] = {swapChain.swapChain};
	PresentInfoKHR presentInfo(1, signalSemaphore, 1, swapChains, &imageIndex);
	Result result2;
	try {
		result2 = deviceManager.presentQueue.presentKHR(presentInfo);
	} catch (vk::OutOfDateKHRError &err) {
		result2 = vk::Result::eErrorOutOfDateKHR;
	} catch (vk::SystemError &err) {
		throw std::runtime_error("failed to present swap chain image!");
	}
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	if(result2 == Result::eErrorOutOfDateKHR || result2 == Result::eSuboptimalKHR || frameBufferResized) {
		frameBufferResized = false;
		return false;
	}
	return true;
}

void vkr::GraphicsRenderer::cleanup(const Device &device) {
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		device.destroySemaphore(imageAvailableSemaphores[i]);
		device.destroySemaphore(renderFinishedSemaphores[i]);
		device.destroyFence(inFlightFences[i]);
	}
}
