#pragma once

#include "../base/Pipeline.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Pipeline : public Pipeline {
public:
    GL3Pipeline(gl3::GLContext& context);
    virtual ~GL3Pipeline() {}

private:
};

END_GFX_NAMESPACE