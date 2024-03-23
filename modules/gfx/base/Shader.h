#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Shader {
public:
    Shader()          = default;
    virtual ~Shader() = default;

    virtual bool build() = 0;
};

END_GFX_NAMESPACE