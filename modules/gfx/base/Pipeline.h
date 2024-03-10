#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Shader;

class Pipeline {
public:
    Pipeline()          = default;
    virtual ~Pipeline() = default;
    virtual Shader* getShader();

    template <typename Handle>
    Handle getHandle() {
        return static_cast<Handle>(getHandleImp());
    }

protected:
    virtual GFX_HANDLE getHandleImp() = 0;
};

END_GFX_NAMESPACE