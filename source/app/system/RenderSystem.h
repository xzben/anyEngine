#pragma once

#include "../System.h"
#include "pipeline.h"
#include "platform/Window.h"

class RenderSystem : public System {
public:
    RenderSystem(Window* window);
    virtual ~RenderSystem();

    virtual void onUpdate(float dt) override;

protected:
    gfx::Device* m_pDevice;
};