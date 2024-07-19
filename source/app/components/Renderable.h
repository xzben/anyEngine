#pragma once
#include "../Component.h"
#include "pipeline.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

class Renderable : public Component {
    DECLARE_RUNTIME_CLASS(Renderable)
public:
    NS_PIPELINE::RenderNode* getRenderNode() { return &m_renderNode; }

protected:
    NS_PIPELINE::RenderNode m_renderNode;
};

END_NS_SCENCE_GRAPH