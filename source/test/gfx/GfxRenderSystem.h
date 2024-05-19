#pragma once

#include "app/system/RenderSystem.h"

class GfxRenderSystem : public RenderSystem {
public:
    GfxRenderSystem(Window* window);
    virtual ~GfxRenderSystem();

protected:
    virtual void onUpdate(float dt) override;

private:
    gfx::SwapChain* m_swapChain{nullptr};
    gfx::RenderPass* m_renderPass{nullptr};
    gfx::Semaphore* m_imageValidSemahore{nullptr};
    gfx::Semaphore* m_renderSemaphore{nullptr};
    gfx::Fence* m_renderFence{nullptr};
    gfx::CommandPool* m_commandPool{nullptr};
    gfx::Queue* m_queue{nullptr};

    std::vector<gfx::DrawSurface*> m_drawSurfaces;
};