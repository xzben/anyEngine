#pragma once

#include "../base/Texture.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Texture : public Texture {
public:
    GLES3Texture(gles::GLContext& context);
    virtual ~GLES3Texture() {}

private:
};

END_GFX_NAMESPACE