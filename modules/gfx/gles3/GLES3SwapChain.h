#pragma once

#include "../base/SwapChain.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3SwapChain : public SwapChain, public GfxObject {
public:
    GLES3SwapChain(gles::GLContext& context);
    virtual ~GLES3SwapChain() {}

private:
};

END_GFX_NAMESPACE