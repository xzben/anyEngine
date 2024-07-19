#pragma once

#include <unordered_set>

#include "../System.h"
#include "RenderPassMgr.h"
#include "pipeline.h"
#include "platform/Window.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

class Camera;
class Renderable;
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

    NS_PIPELINE::RenderPipeline* getPipeline(NS_PIPELINE::RenderPath* renderPath);

    void culling(Camera* camera, std::vector<NS_PIPELINE::RenderNode*>& renderables);

protected:
    gfx::Device* m_pDevice;
    Window* m_window;

    std::unordered_set<Renderable*> m_renderables;
    std::vector<Camera*> m_cameras;
    std::unordered_map<NS_PIPELINE::RenderPath*, NS_PIPELINE::RenderPipeline*> m_pipelines;
    NS_PIPELINE::RenderPassMgr m_renderPassMgr;
    std::vector<NS_PIPELINE::RenderNode*> m_curRenderNodes;
};

END_NS_SCENCE_GRAPH