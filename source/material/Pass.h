#pragma once

#include <string>

#include "Effect.h"
#include "common/Object.h"
#include "gfx.h"
#include "sceneGraphDefine.h"
BEGIN_NS_SCENCE_GRAPH

class Pass : public Object {
    DECLARE_RUNTIME_CLASS(Pass)
public:
    Pass(const std::string& name, Effect* effect);
    virtual ~Pass();

    gfx::PipelineState& getPipelineState() { return m_pipelineState; }
    const gfx::PipelineState& getPipelineState() const { return m_pipelineState; }

    const std::string& getName() const { return m_name; }
    Effect* getEffect() const { return m_effect; }

protected:
    std::string m_name;
    Effect* m_effect{nullptr};
    gfx::PipelineState m_pipelineState;
};

END_NS_SCENCE_GRAPH