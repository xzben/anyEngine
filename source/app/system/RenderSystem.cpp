#include "RenderSystem.h"

#include "app/components/Camera.h"
#include "app/components/Renderable.h"
BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(RenderSystem)

RenderSystem::RenderSystem(Window* window) : m_window(window) {}

RenderSystem::~RenderSystem() {}

NS_PIPELINE::RenderPipeline* RenderSystem::getPipeline(NS_PIPELINE::RenderPath* renderPath) {
    auto it                               = m_pipelines.find(renderPath);
    NS_PIPELINE::RenderPipeline* pipeline = nullptr;
    if (it == m_pipelines.end()) {
        pipeline                = NS_PIPELINE::RenderPipeline::create(*m_pDevice, m_renderPassMgr);
        m_pipelines[renderPath] = pipeline;
    } else {
        pipeline = it->second;
    }

    return pipeline;
}

void RenderSystem::culling(Camera* camera, std::vector<NS_PIPELINE::RenderNode*>& nodes) {
    nodes.clear();
    for (auto& renderable : m_renderables) {
        nodes.push_back(renderable->getRenderNode());
    }
}
void RenderSystem::onUpdate(float dt) {
    m_curRenderNodes.reserve(m_renderables.size());

    for (auto& camera : m_cameras) {
        auto renderPath   = camera->getRenderPath();
        auto renderTarget = camera->getRenderTarget();
        auto pipeline     = getPipeline(renderPath);

        pipeline->setup(renderPath, renderTarget);
        culling(camera, m_curRenderNodes);
        pipeline->render(m_curRenderNodes, camera->getRenderView());
    }
}

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