#include "GfxRenderSystem.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(GfxRenderSystem)

GfxRenderSystem::GfxRenderSystem(Window* window) : RenderSystem(window) {
    auto hwnd = window->getNativeWinHandle();
    auto size = window->getSize();

    gfx::SurfaceInfo info;
    info.handle          = hwnd;
    info.width           = size.width;
    info.height          = size.height;
    info.singleBuffer    = false;
    m_swapChain          = m_pDevice->createSwapChain(info, true);
    m_queue              = m_pDevice->getQueue(gfx::QueueType::Graphics, 0);
    m_commandPool        = m_queue->createPool(gfx::ResetMode::ResetPool);
    m_renderFence        = m_pDevice->createFence(true);
    m_renderSemaphore    = m_pDevice->createSemaphore();
    m_imageValidSemahore = m_pDevice->createSemaphore();

    std::vector<gfx::Attachment> attachments;
    attachments.resize(2);

    attachments[0].format = gfx::PixelFormat::RGBA8;
    attachments[0].loadOp = gfx::LoadOp::CLEAR;

    attachments[1].format        = gfx::PixelFormat::Depth24Stencil8;
    attachments[1].loadOp        = gfx::LoadOp::CLEAR;
    attachments[1].stencilLoadOp = gfx::LoadOp::CLEAR;

    gfx::SubPass subpass;

    subpass.colorAttachments.push_back({0, gfx::ImageLayout::COLOR_ATTACHMENT_OPTIMAL});
    subpass.depthStencilAttachments.push_back(
        {1, gfx::ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL});

    m_renderPass = m_pDevice->createRenderPass(attachments, {subpass}, {});

    uint32_t imageCount = m_swapChain->getImageCount();
    m_drawColorSurfaces.resize(imageCount);
    m_drawDepthStencilSurfaces.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; i++) {
        m_drawColorSurfaces[i] =
            m_pDevice->createDrawSurface(m_swapChain, i, gfx::SwapChainAttachment::COLOR);
        m_drawDepthStencilSurfaces[i] =
            m_pDevice->createDrawSurface(m_swapChain, i, gfx::SwapChainAttachment::DEPTH_STENCIL);
    }

    window->addEventListener(EventType::WINDOW_RESIZE, [&](const Event& event) {
        auto resizeEvent = event.cast<WindowResizeEvent>();

        gfx::SurfaceInfo info = m_swapChain->getInfo();
        info.width            = resizeEvent.width;
        info.height           = resizeEvent.height;
        m_swapChain->handleUpdateSurfaceInfo(info);
    });
}

GfxRenderSystem::~GfxRenderSystem() {
    if (m_swapChain != nullptr) {
        m_pDevice->destroyObject(m_swapChain);
        m_swapChain = nullptr;
    }

    if (m_commandPool) {
        m_pDevice->destroyObject(m_commandPool);
        m_commandPool = nullptr;
    }

    if (m_renderFence) {
        m_pDevice->destroyObject(m_renderFence);
        m_renderFence = nullptr;
    }

    if (m_renderSemaphore) {
        m_pDevice->destroyObject(m_renderSemaphore);
        m_renderSemaphore = nullptr;
    }

    if (m_imageValidSemahore) {
        m_pDevice->destroyObject(m_imageValidSemahore);
        m_imageValidSemahore = nullptr;
    }

    if (m_renderPass) {
        m_pDevice->destroyObject(m_renderPass);
        m_renderPass = nullptr;
    }

    for (auto& item : m_drawColorSurfaces) {
        m_pDevice->destroyObject(item);
    }
    m_drawColorSurfaces.clear();

    for (auto& item : m_drawDepthStencilSurfaces) {
        m_pDevice->destroyObject(item);
    }
    m_drawDepthStencilSurfaces.clear();
}

void GfxRenderSystem::draw(gfx::CommandBuffer* cmd) {}

void GfxRenderSystem::onUpdate(float dt) {
    auto waitResult = m_renderFence->wait(1000);
    if (waitResult == gfx::WaitResult::TIMEOUT) return;

    auto result         = m_swapChain->acquireNextImage(m_imageValidSemahore, nullptr, 0);
    uint32_t imageIndex = result.second;
    m_renderFence->reset();
    m_swapChain->setActiveImageIndex(imageIndex);
    m_commandPool->reset();
    auto cmd = m_commandPool->alloc(gfx::CommandBufferLevel::PRIMARY);

    gfx::DrawSurface* pSurfaces[2];
    pSurfaces[0] = m_drawColorSurfaces[imageIndex];
    pSurfaces[1] = m_drawDepthStencilSurfaces[imageIndex];
    cmd->begin(gfx::CommandBufferUsage::ONE_TIME_SUBMIT);

    auto info = m_swapChain->getInfo();
    gfx::ClearValue clear[2];
    clear[0].color        = {1.0, 0.0, 0.0, 1.0};
    clear[1].depthStencil = {1.0, 0};

    gfx::BeginRenderPassInfo beginInfo;
    beginInfo.renderPass   = m_renderPass;
    beginInfo.surfaceCount = 2;
    beginInfo.pSurfaces    = pSurfaces;
    beginInfo.pClearValues = clear;
    beginInfo.viewport     = {0, 0, info.width, info.height};
    cmd->beginRendPass(beginInfo);
    cmd->end();

    m_queue->submit({cmd}, {m_imageValidSemahore}, {m_renderSemaphore}, m_renderFence);
    m_queue->present(m_swapChain, imageIndex, {m_renderSemaphore});
}

END_NS_SCENCE_GRAPH