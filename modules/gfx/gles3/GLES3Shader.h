#pragma once

#include "../base/Shader.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Shader : public Shader, public GfxObject {
public:
    GLES3Shader(gles::GLContext& context);
    virtual ~GLES3Shader() {}

private:
};

END_GFX_NAMESPACE