#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
BEGIN_GFX_NAMESPACE

class Shader;

class Pipeline : public Handle, public GfxObject {
public:
    virtual Shader* getShader() = 0;

    virtual const PipelineState& getState() const = 0;

protected:
    Pipeline() : GfxObject(GfxObjectType::Pipeline) {}
    virtual ~Pipeline() = default;
};

END_GFX_NAMESPACE