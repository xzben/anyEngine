#pragma once

#include "app/system/RenderSystem.h"

class GfxRenderSystem : public RenderSystem {
public:
    GfxRenderSystem(Window* window);
    virtual ~GfxRenderSystem();

protected:
    virtual void onUpdate(float dt) override;
};