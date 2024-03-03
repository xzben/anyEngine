#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Shader;

class Pipeline {
public:
    virtual Shader* getShader();
};

END_GFX_NAMESPACE