#include "RenderSystem.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(RenderSystem)

RenderSystem::RenderSystem(Window* window) : m_window(window) {}

RenderSystem::~RenderSystem() {}
void RenderSystem::onUpdate(float dt) {}

void RenderSystem::init() {
    gfx::DeviceInfo deviceInfo;
    deviceInfo.api                  = gfx::GFX_API::GL3;
    auto& size                      = m_window->getSize();
    deviceInfo.surface.handle       = m_window->getNativeWinHandle();
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
    RuntimeSuperType::init();
}

void RenderSystem::onInit() {}
void RenderSystem::onUnInit() {}

END_NS_SCENCE_GRAPH