#pragma once

#include "../base/Texture.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Texture : public Texture {
public:
    GL3Texture(gl3::GLContext& context);
    virtual ~GL3Texture() {}

private:
};

END_GFX_NAMESPACE