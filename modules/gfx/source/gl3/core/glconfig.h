#pragma once

#include "common/gfx_common.h"
#include "platformDefine.h"

#define OPENG_UNKNOWN 0
#define OPENGL_AGL 1
#define OPENGL_EGL 2
#define OPENGL_WGL 3

#define CUR_GL_TYPE OPENG_UNKNOWN

#if CUR_PLATFORM == PLATFORM_WINDOWS
#undef CUR_GL_TYPE
#define CUR_GL_TYPE OPENGL_WGL
#elif CUR_PLATFORM == PLATFORM_ANDROID
#undef CUR_GL_TYPE
#define CUR_GL_TYPE OPENGL_EGL
#elif CUR_PLATFORM == PLATFORM_IOS || CUR_PLATFORM == PLATFORM_MAC
#undef CUR_GL_TYPE
#define CUR_GL_TYPE OPENGL_AGL
#endif