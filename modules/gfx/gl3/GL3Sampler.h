#pragma once

#include "../base/Sampler.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Sampler : public Sampler {
public:
    GL3Sampler(gl3::GLContext& context);
    virtual ~GL3Sampler() {}

private:
};

END_GFX_NAMESPACE