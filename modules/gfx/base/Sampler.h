#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Sampler {
public:
    Sampler()          = default;
    virtual ~Sampler() = default;
};

END_GFX_NAMESPACE