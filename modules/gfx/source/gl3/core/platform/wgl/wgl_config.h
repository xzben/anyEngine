#pragma once

#include "../../glconfig.h"

#if CUR_GL_TYPE == OPENGL_WGL
#include "GL/glew.h"
#include "GL/wglew.h"
#include "Windows.h"
#include "gl3/GL3SwapChain.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

using WGLBool = int;

#define WGL_TRUE 1
#define WGL_FALSE 0
#define WGL_DONT_CARE -1

struct WGLConfig {
    int redBits{8};
    int greenBits{8};
    int blueBits{8};
    int alphaBits{8};
    int depthBits{24};
    int stencilBits{8};
    int accumRedBits{0};
    int accumGreenBits{0};
    int accumBlueBits{0};
    int accumAlphaBits{0};
    int auxBuffers{0};
    WGLBool stereo{WGL_FALSE};
    int samples{0};
    WGLBool sRGB{WGL_FALSE};
    WGLBool doublebuffer{WGL_TRUE};
    WGLBool transparent{WGL_FALSE};
    uintptr_t handle{0};
};

enum class ProfileType {
    OPENGL_ANY_PROFILE = 0,
    OPENGL_CORE_PROFILE,
    OPENGL_COMPAT_PROFILE,
};

enum class RobustnessType {
    NO_ROBUSTNESS = 0,
    NO_RESET_NOTIFICATION,
    LOSE_CONTEXT_ON_RESET,
};

enum class ReleaseType {
    ANY_RELEASE_BEHAVIOR = 0,
    RELEASE_BEHAVIOR_FLUSH,
    RELEASE_BEHAVIOR_NONE,
};

struct WLGCtxconfig {
    int major{4};
    int minor{3};
    WGLBool forward{WGL_TRUE};
    WGLBool debug{WGL_TRUE};
    WGLBool noerror{WGL_TRUE};
    ProfileType profile{ProfileType::OPENGL_CORE_PROFILE};
    RobustnessType robustness{RobustnessType::NO_ROBUSTNESS};
    ReleaseType release{ReleaseType::ANY_RELEASE_BEHAVIOR};
};

extern int choosePixelFormatWGL(HDC hdc, const WGLConfig* fbconfig);
extern void setAttribsARB(int* attribs, const WLGCtxconfig& ctxConfig);

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE

#endif  // #if CUR_GL_TYPE == OPENGL_WGL