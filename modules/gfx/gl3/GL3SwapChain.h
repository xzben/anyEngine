#pragma once

#include "../base/SwapChain.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3SwapChain : public SwapChain {
public:
    GL3SwapChain(gl3::GLContext& context);
    virtual ~GL3SwapChain() {}

private:
};

END_GFX_NAMESPACE