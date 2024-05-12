#pragma once

#include "glconfig.h"

#if CUR_GL_TYPE == OPENGL_WGL
#include "platform/wgl/wgl.h"
#elif CUR_GL_TYPE == OPENGL_AGL
#include "platform/agl/agl.h"
#elif CUR_GL_TYPE == OPENGL_EGL
#include "platform/egl/egl.h"
#else
static_assert(false);  // unsupport OPEN TYPE
#endif