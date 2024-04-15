#pragma once

#include "../base/Sampler.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Sampler : public Sampler {
public:
    GLES3Sampler(gles::GLContext& context);
    virtual ~GLES3Sampler() {}

private:
};

END_GFX_NAMESPACE