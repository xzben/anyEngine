#pragma once

#include "../../glconfig.h"

#if CUR_GL_TYPE == OPENGL_WGL
#include "GL/glew.h"
#include "GL/wglew.h"
#include "Windows.h"
#include "gl3/GL3SwapChain.h"

#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "opengl32.lib")

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

#if DEBUG_OPENGL

#define WGL_CHECK(code)                               \
    do {                                              \
        code;                                         \
        const DWORD error = GetLastError();           \
        if (error != 0) {                             \
            CCERROR("wgl %s error:%x", #code, error); \
        }                                             \
    } while (false)
#else
#define WGL_CHECK(code) code
#endif

struct WGlContext {
    HDC hdc{NULL};
    HGLRC hglrc{NULL};
    HWND hwnd{NULL};
    WGlContext* shareContext{nullptr};

    bool selfHdc;
};

class WGLSwapChain : public GL3SwapChain {
public:
    // 基于一个mainContext 创建
    WGLSwapChain(GL3Device& device, const SurfaceInfo& info, bool needDepthStencil,
                 WGlContext* shareContext);

    virtual ~WGLSwapChain();
    virtual void handleUpdateSurfaceInfo(const SurfaceInfo& info) override;

    WGlContext* getContext() { return m_context; }
    void swapBuffer() override;

private:
    WGlContext* m_context{nullptr};
};

class WGLHelper {
public:
    static bool init();
    static WGlContext* createContext(HWND window, WGlContext* share, bool singleBuffer);
    static void deleteContext(WGlContext* context);
    static WGLSwapChain* createWindowSurface(GL3Device& device, const SurfaceInfo& info,
                                             bool needDepthStencil, WGlContext* mainContext);

    static void makeCurrent(WGlContext* context);
    static void deleteWindowSurface(WGLSwapChain* surface);
    static void swapBuffer(WGlContext* context);
    static WGlContext* getLastContext();
    static void exitCurrent(WGlContext* context);
};

using GLContextType = WGlContext*;

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE

#endif  // #if CUR_GL_TYPE == OPENGL_WGL