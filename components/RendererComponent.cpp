#include "RendererComponent.h"

vkr::RendererComponent::RendererComponent(const vkr::DeviceManager &deviceManager) : deviceManager(deviceManager), device(*deviceManager.device) {}

void vkr::RendererComponent::init() {}

void vkr::RendererComponent::cleanup() {}
