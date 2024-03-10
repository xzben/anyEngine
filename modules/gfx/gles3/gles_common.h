#pragma once

#include <glad/glad.h>

#include "../common/gfx_common.h"
#include "core/gles3_core.h"
#include "gles3_types.h"

using OGL_HANDLE                 = uint32_t;
const OGL_HANDLE OGL_NULL_HANDLE = 0;

#ifndef NDEBUG
#define GL_CHECK(x)                                         \
    do {                                                    \
        x;                                                  \
        GLenum err = glGetError();                          \
        if (err != GL_NO_ERROR) {                           \
            CCERROR("%s returned GL error: 0x%x", #x, err); \
        }                                                   \
    } while (0)

#define EGL_CHECK(x)                                         \
    do {                                                     \
        x;                                                   \
        EGLint err = eglGetError();                          \
        if (err != EGL_SUCCESS) {                            \
            CCERROR("%s returned EGL error: 0x%x", #x, err); \
        }                                                    \
    } while (0)
#else
#define GL_CHECK(x) x
#define EGL_CHECK(x) x
#endif  // !NDEBUG