#pragma once

#include "../base/RenderPass.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3RenderPass : public RenderPass {
public:
    GLES3RenderPass(gles::GLContext& context);
    virtual ~GLES3RenderPass() {}

private:
};

END_GFX_NAMESPACE