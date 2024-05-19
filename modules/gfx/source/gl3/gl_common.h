#pragma once

#include "GL/glew.h"
#include "common/gfx_common.h"
#include "core/gl3_core.h"
#include "gl3_types.h"

BEGIN_GFX_NAMESPACE
using OGL_HANDLE                 = uint32_t;
const OGL_HANDLE OGL_NULL_HANDLE = 0;

#define DEBUG_OPENGL true
#define OPENGL_RESOURCE_ANSC false

#if DEBUG_OPENGL

#define GL_CHECK(x)                                       \
    do {                                                  \
        x;                                                \
        GLenum err = glGetError();                        \
        if (err != GL_NO_ERROR) {                         \
            CCERROR("opengl code: %s error:%d", #x, err); \
        }                                                 \
    } while (0)
#else
#define GL_CHECK(x) x
#define EGL_CHECK(x) x
#endif  // !NDEBUG

END_GFX_NAMESPACE