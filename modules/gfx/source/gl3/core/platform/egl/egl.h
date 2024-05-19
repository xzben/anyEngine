#pragma once

#include "../../glconfig.h"

#if CUR_GL_TYPE == OPENGL_EGL
#include "GL/glew.h"
#if DEBUG_OPENGL
#define EGL_CHECK(x)                                         \
    do {                                                     \
        x;                                                   \
        EGLint err = eglGetError();                          \
        if (err != EGL_SUCCESS) {                            \
            CCERROR("%s returned EGL error: 0x%x", #x, err); \
        }                                                    \
    } while (0)
#else
#define EGL_CHECK(x) x
#endif
BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE

#endif  // CUR_GL_TYPE == OPENGL_EGL