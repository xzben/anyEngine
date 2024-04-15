#pragma once

#include "../../glconfig.h"

#if CUR_GL_TYPE == OPENGL_WGL
#include "GL/glew.h"
#include "GL/wglew.h"
#include "Windows.h"
#include "gl3/GL3SwapChain.h"

BEGIN_GFX_NAMESPACE

struct WGlContext {
    HDC hdc{NULL};
    HGLRC hglrc{NULL};
    HWND hwnd{NULL};
    WGlContext* share{nullptr};

    bool selfHdc;
};

class WGLSwapChain : public GL3SwapChain {};

class WGLHelper {
public:
    static bool init();
    static WGlContext* createContext(HWND window, WGlContext* share);
    static void deleteContext(WGlContext* context);
    static WGLSwapChain* createWindowSurface(void* win, uint32_t width,
                                             uint32_t height);
    static void makeCurrent(WGlContext* context,
                            WGLSwapChain* surface = nullptr);
    static void deleteWindowSurface(WGLSwapChain* surface);
    static void swapBuffer(WGlContext* context);
};
END_GFX_NAMESPACE

#endif  // #if CUR_GL_TYPE == OPENGL_WGL