#pragma once

#include "../base/RenderPass.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3RenderPass : public RenderPass {
public:
    GL3RenderPass(gl3::GLContext& context);
    virtual ~GL3RenderPass() {}

private:
};

END_GFX_NAMESPACE