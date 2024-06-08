#pragma once

#include "app/system/RenderSystem.h"

BEGIN_NS_SCENCE_GRAPH
class GfxRenderSystem : public RenderSystem {
public:
    GfxRenderSystem(Window* window);
    virtual ~GfxRenderSystem();

protected:
    virtual void onUpdate(float dt) override;

    void draw(gfx::CommandBuffer* cmd);

private:
    gfx::SwapChain* m_swapChain{nullptr};
    gfx::RenderPass* m_renderPass{nullptr};
    gfx::Semaphore* m_imageValidSemahore{nullptr};
    gfx::Semaphore* m_renderSemaphore{nullptr};
    gfx::Fence* m_renderFence{nullptr};
    gfx::CommandPool* m_commandPool{nullptr};
    gfx::Queue* m_queue{nullptr};

    std::vector<gfx::DrawSurface*> m_drawColorSurfaces;
    std::vector<gfx::DrawSurface*> m_drawDepthStencilSurfaces;
};

END_NS_SCENCE_GRAPH