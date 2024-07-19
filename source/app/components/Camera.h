#pragma once
#include "../Component.h"
#include "pipeline.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

class Camera : public Component {
    DECLARE_RUNTIME_CLASS(Camera)
public:
    NS_PIPELINE::RenderPath* getRenderPath() const { return m_renderPath; }
    NS_PIPELINE::RenderTarget* getRenderTarget() const { return m_renderTarget; }
    NS_PIPELINE::RenderView& getRenderView() { return m_renderView; }

protected:
    NS_PIPELINE::RenderView m_renderView;
    NS_PIPELINE::RenderPath* m_renderPath;
    NS_PIPELINE::RenderTarget* m_renderTarget{nullptr};
};

END_NS_SCENCE_GRAPH