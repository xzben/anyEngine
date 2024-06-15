#pragma once

#include "../System.h"
#include "pipeline.h"
#include "platform/Window.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH
class RenderSystem : public System {
    DECLARE_RUNTIME_CLASS(RenderSystem)
public:
    RenderSystem(Window* window);
    virtual ~RenderSystem();

    gfx::Device* getDevice() { return m_pDevice; }

protected:
    void init() override;
    virtual void onUpdate(float dt) override;
    virtual void onInit() override;
    virtual void onUnInit() override;

protected:
    gfx::Device* m_pDevice;
    Window* m_window;
};

END_NS_SCENCE_GRAPH