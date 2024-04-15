#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Shader;

class Pipeline: public GfxObject {
public:
    virtual Shader* getShader();

    virtual const PipelineState& getState() = 0;

    template <typename Handle>
    Handle getHandle() {
        return static_cast<Handle>(getHandleImp());
    }

protected:
    Pipeline() : GfxObject(GfxObjectType::Pipeline) {}
    virtual ~Pipeline()               = default;
    virtual GFX_HANDLE getHandleImp() = 0;
};

END_GFX_NAMESPACE