#include "RenderSystem.h"

RenderSystem::RenderSystem(Window* window) {
    gfx::DeviceInfo deviceInfo;
    deviceInfo.api                  = gfx::GFX_API::GL3;
    auto& size                      = window->getSize();
    deviceInfo.surface.handle       = window->getNativeWinHandle();
    deviceInfo.surface.width        = size.width;
    deviceInfo.surface.height       = size.height;
    deviceInfo.surface.singleBuffer = false;
    deviceInfo.queues.push_back({
        gfx::QueueType::Present,
        1.0,
        true,
    });
    deviceInfo.queues.push_back({
        gfx::QueueType::Graphics,
        1.0,
        true,
    });
    deviceInfo.queues.push_back({
        gfx::QueueType::Graphics,
        1.0,
        true,
    });
    deviceInfo.queues.push_back({
        gfx::QueueType::Compute,
        1.0,
        true,
    });

    m_pDevice = gfx::Device::createDevice(deviceInfo);
}

RenderSystem::~RenderSystem() {}
void RenderSystem::onUpdate(float dt) {}