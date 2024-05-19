#include "GfxRenderSystem.h"

GfxRenderSystem::GfxRenderSystem(Window* window) : RenderSystem(window) {
    auto hwnd = window->getNativeWinHandle();
    auto size = window->getSize();

    m_swapChain          = m_pDevice->createSwapChain(hwnd, size.width, size.height, false, true);
    m_queue              = m_pDevice->getQueue(gfx::QueueType::Graphics, 0);
    m_commandPool        = m_queue->createPool(gfx::ResetMode::ResetPool);
    m_renderFence        = m_pDevice->createFence(true);
    m_renderSemaphore    = m_pDevice->createSemaphore();
    m_imageValidSemahore = m_pDevice->createSemaphore();
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
}

void GfxRenderSystem::onUpdate(float dt) {
    auto waitResult = m_renderFence->wait(1000);
    if (waitResult == gfx::WaitResult::TIMEOUT) return;

    auto result = m_swapChain->acquireNextImage(m_imageValidSemahore, nullptr, 0);
    m_renderFence->reset();
    m_swapChain->setActiveImageIndex(result.second);
}