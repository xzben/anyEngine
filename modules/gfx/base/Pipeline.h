#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
BEGIN_GFX_NAMESPACE

class Shader;

class Pipeline : public Handle, public GfxObject {
public:
    virtual Shader* getShader();

    virtual const PipelineState& getState() = 0;

protected:
    Pipeline() : GfxObject(GfxObjectType::Pipeline) {}
    virtual ~Pipeline() = default;
};

END_GFX_NAMESPACE