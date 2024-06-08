#pragma once

#include "../System.h"
#include "pipeline.h"
#include "platform/Window.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH
class RenderSystem : public System {
public:
    RenderSystem(Window* window);
    virtual ~RenderSystem();

    virtual void onUpdate(float dt) override;

    gfx::Device* getDevice() { return m_pDevice; }

protected:
    gfx::Device* m_pDevice;
};

END_NS_SCENCE_GRAPH