#pragma once

#include "../base/Fence.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Fence : public Fence {
public:
    GL3Fence(gl3::GLContext& context);
    virtual ~GL3Fence() {}

private:
};

END_GFX_NAMESPACE