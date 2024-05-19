#include "GfxRenderSystem.h"

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

    gfx::Attachment attachment;
    attachment.load_op = gfx::LoadOp::CLEAR;
    gfx::SubPass subpass;

    subpass.colorAttachments.push_back({0, gfx::ImageLayout::COLOR_ATTACHMENT_OPTIMAL});
    m_renderPass = m_pDevice->createRenderPass({attachment}, {subpass}, {});

    uint32_t imageCount = m_swapChain->getImageCount();
    m_drawSurfaces.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; i++) {
        m_drawSurfaces[i] = m_pDevice->createDrawSurface(m_swapChain, i, true);
    }
}

GfxRenderSystem::~GfxRenderSystem() {
    if (m_swapChain != nullptr) {
        m_pDevice->destroySwapChain(m_swapChain);
        m_swapChain = nullptr;
    }

    if (m_commandPool) {
        m_queue->destroyPool(m_commandPool);
        m_commandPool = nullptr;
    }

    if (m_renderFence) {
        m_pDevice->destroyFence(m_renderFence);
        m_renderFence = nullptr;
    }

    if (m_renderSemaphore) {
        m_pDevice->destroySemaphore(m_renderSemaphore);
        m_renderSemaphore = nullptr;
    }

    if (m_imageValidSemahore) {
        m_pDevice->destroySemaphore(m_imageValidSemahore);
        m_imageValidSemahore = nullptr;
    }

    if (m_renderPass) {
        m_pDevice->destroyRenderPasss(m_renderPass);
        m_renderPass = nullptr;
    }

    for (auto& item : m_drawSurfaces) {
        m_pDevice->destroyDrawSurface(item);
    }
    m_drawSurfaces.clear();
}

void GfxRenderSystem::onUpdate(float dt) {
    auto waitResult = m_renderFence->wait(1000);
    if (waitResult == gfx::WaitResult::TIMEOUT) return;

    auto result         = m_swapChain->acquireNextImage(m_imageValidSemahore, nullptr, 0);
    uint32_t imageIndex = result.second;
    m_renderFence->reset();
    m_swapChain->setActiveImageIndex(imageIndex);
    m_commandPool->reset();
    auto cmd = m_commandPool->alloc(gfx::CommandBufferLevel::PRIMARY);

    auto surface = m_drawSurfaces[imageIndex];
    cmd->begin(gfx::CommandBufferUsage::ONE_TIME_SUBMIT);

    auto info = m_swapChain->getInfo();
    gfx::ClearValue clear;
    clear.color = {1.0, 0.0, 0.0, 1.0};
    cmd->setViewport(0, 0, info.width, info.height);
    cmd->beginRendPass(m_renderPass, {surface}, {clear});
    cmd->end();

    m_queue->submit({cmd}, {m_imageValidSemahore}, {m_renderSemaphore}, m_renderFence);
    m_queue->present(m_swapChain, imageIndex, {m_renderSemaphore});
}