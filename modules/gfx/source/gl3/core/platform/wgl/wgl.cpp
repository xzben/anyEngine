#include "wgl.h"

#include <conio.h>
#include <windows.h>

#include <cstdio>
#include <mutex>

#include "wgl_config.h"

#if CUR_GL_TYPE == OPENGL_WGL

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

static std::mutex g_context_lock;

bool WGLHelper::init() {
    auto tempCtx = createContext(NULL, nullptr, true);
    makeCurrent(tempCtx);
    glewInit();
    makeCurrent(nullptr);
    deleteContext(tempCtx);
    return true;
}

WGlContext* WGLHelper::createContext(HWND window, WGlContext* shareContext, bool singleBuffer) {
    g_context_lock.lock();

    bool success = true;
    int attribs[40];
    int pixelFormat;
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    HGLRC share = NULL;
    if (shareContext != nullptr) {
        share = shareContext->hglrc;
    }
    HDC hdc        = NULL;
    HGLRC newHglrc = NULL;
    bool selfHdc   = false;
    if (window == NULL) {
        HWND forageHwnd = NULL;
        hdc             = ::GetDC(forageHwnd);
        selfHdc         = true;
    } else {
        hdc = ::GetDC(window);
    }

    WLGCtxconfig ctxConfig;
    WGLConfig pbConfig;
    pbConfig.doublebuffer = singleBuffer ? WGL_FALSE : WGL_TRUE;

    pixelFormat = choosePixelFormatWGL(hdc, &pbConfig);
    if (0 == pixelFormat) {
        CCERROR("can't find pixelFormat for hdc:%x", hdc);
        return nullptr;
    }
    if (!DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd)) {
        const DWORD error = GetLastError();
        CCERROR("WGL: Failed to retrieve PFD for selected pixel format error:%d", error);
        return nullptr;
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
        const DWORD error = GetLastError();
        CCERROR("WGL: Failed to set selected pixel format by error:%x", error);
        return nullptr;
    }

    if (WGLEW_ARB_create_context) {
        setAttribsARB(attribs, ctxConfig);
        newHglrc = wglCreateContextAttribsARB(hdc, share, attribs);
        if (!newHglrc) {
            const DWORD error = GetLastError();
            CCERROR("create context error[%x]", error);
            success = false;
        }
    } else {
        newHglrc = wglCreateContext(hdc);
        if (!newHglrc) {
            const DWORD error = GetLastError();
            CCERROR("create context error[%d]", error);
            success = false;
        }
        if (share) {
            if (!wglShareLists(newHglrc, share)) {
                CCERROR("failed to share context[%d|%d]", share, newHglrc);
                success = false;
            }
        }
    }

    g_context_lock.unlock();

    assert(success);
    if (success) {
        WGlContext* context   = new WGlContext();
        context->selfHdc      = selfHdc;
        context->hdc          = hdc;
        context->hglrc        = newHglrc;
        context->shareContext = shareContext;
        context->hwnd         = window;
        return context;
    } else {
        return nullptr;
    }
}

void WGLHelper::deleteContext(WGlContext* context) {
    if (context == nullptr) return;

    if (context->hglrc) {
        wglDeleteContext(context->hglrc);
        context->hglrc = NULL;
    }
    delete context;
}

thread_local WGlContext* s_lastContext = nullptr;
thread_local WGlContext* s_preContext  = nullptr;

WGlContext* WGLHelper::getLastContext() {
    g_context_lock.lock();
    WGlContext* lastContext = s_lastContext;
    g_context_lock.unlock();

    return lastContext;
}

void WGLHelper::makeCurrent(WGlContext* context) {
    if (s_lastContext == context) {
        return;
    }

    if (context) {
        g_context_lock.lock();
        if (!wglMakeCurrent(context->hdc, context->hglrc)) {
            g_context_lock.unlock();
            CCERROR("WGL: Failed to make context[ hdc: %d | hglrc: %d] current", context->hdc,
                    context->hglrc);
        } else {
            s_preContext  = s_lastContext;
            s_lastContext = context;
            g_context_lock.unlock();
        }

    } else {
        g_context_lock.lock();
        if (!wglMakeCurrent(NULL, NULL)) {
            g_context_lock.unlock();
            CCERROR("WGL: Failed to make context empty");
        } else {
            s_preContext  = s_lastContext;
            s_lastContext = context;
            g_context_lock.unlock();
        }
    }
}

void WGLHelper::exitCurrent(WGlContext* context) {
    if (s_lastContext != context) {
        return;
    }

    g_context_lock.lock();
    if (!wglMakeCurrent(NULL, NULL)) {
        g_context_lock.unlock();
        CCERROR("WGL: Failed to make context empty");
    } else {
        g_context_lock.unlock();
        s_lastContext = nullptr;
    }
}

WGLSwapChain* WGLHelper::createWindowSurface(GL3Device& device, const SurfaceInfo& info,
                                             bool needDepthStencil, WGlContext* mainContext) {
    return new WGLSwapChain(device, info, needDepthStencil, mainContext);
}

void WGLHelper::deleteWindowSurface(WGLSwapChain* surface) { delete surface; }
void WGLHelper::swapBuffer(WGlContext* context) { SwapBuffers(context->hdc); }

//-----

WGLSwapChain::WGLSwapChain(GL3Device& device, const SurfaceInfo& info, bool needDepthStencil,
                           WGlContext* shareContext)
    : GL3SwapChain(device, info, needDepthStencil) {
    m_context = WGLHelper::createContext((HWND)info.handle, shareContext, info.singleBuffer);
}

void WGLSwapChain::handleUpdateSurfaceInfo(const SurfaceInfo& info) {
    WGlContext* shareContext = m_context->shareContext;
    WGLHelper::deleteContext(m_context);

    m_info = info;

    m_context = WGLHelper::createContext((HWND)info.handle, shareContext, m_info.singleBuffer);

    updateAttachment(info.width, info.height);
}

void WGLSwapChain::swapBuffer() { WGLHelper::swapBuffer(m_context); }

WGLSwapChain::~WGLSwapChain() {
    if (m_context) {
        WGLHelper::deleteContext(m_context);
    }
    m_context = nullptr;
}

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE

#endif  // #if CUR_GL_TYPE == OPENGL_WGL