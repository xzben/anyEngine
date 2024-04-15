#pragma once

#include "../../common/gfx_common.h"
#include "gl3_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class GLContext {
public:
    GLContext() {}
    virtual ~GLContext() {}
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE