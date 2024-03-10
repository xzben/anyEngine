#pragma once

#include "../base/Fence.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Fence : public Fence, public GfxObject {
public:
    GLES3Fence(gles::GLContext& context);
    virtual ~GLES3Fence() {}

private:
};

END_GFX_NAMESPACE