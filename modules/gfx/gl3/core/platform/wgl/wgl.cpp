#include "wgl.h"

#include <mutex>

#include "wgl_config.h"

#if CUR_GL_TYPE == OPENGL_WGL

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

static std::mutex g_context_lock;

bool WGLHelper::init() {
    HDC hdc = ::GetDC(NULL);
    g_context_lock.lock();
    HGLRC tempHglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempHglrc);
    glewInit();
    wglMakeCurrent(NULL, NULL);
    g_context_lock.unlock();
    wglDeleteContext(tempHglrc);
    return true;
}

WGlContext* WGLHelper::createContext(HWND window, WGlContext* shareContext,
                                     bool singleBuffer) {
    bool success = true;
    int attribs[40];
    int pixelFormat;
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC share = NULL;
    if (shareContext != nullptr) {
        share = shareContext->hglrc;
    }
    HDC hdc        = NULL;
    HGLRC newHglrc = NULL;
    bool selfHdc   = false;
    if (window == NULL) {
        hdc     = ::GetDC(NULL);
        selfHdc = true;
    } else {
        hdc = ::GetDC(window);
    }
    WLGCtxconfig ctxConfig;
    WGLConfig pbConfig;
    pbConfig.doublebuffer = singleBuffer ? WGL_FALSE : WGL_TRUE;

    pixelFormat = choosePixelFormatWGL(hdc, &pbConfig);
    if (0 == pixelFormat) {
        CCERROR("can't find pixelFormat for hdc:%d", hdc);
        return nullptr;
    }
    if (!DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd)) {
        CCERROR("WGL: Failed to retrieve PFD for selected pixel format");
        return nullptr;
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
        CCERROR("WGL: Failed to set selected pixel format");
        return nullptr;
    }

    g_context_lock.lock();

    if (WGLEW_ARB_create_context) {
        setAttribsARB(attribs, ctxConfig);
        newHglrc = wglCreateContextAttribsARB(hdc, share, attribs);
        if (!newHglrc) {
            const DWORD error = GetLastError();
            CCERROR("create context error[%d]", error);
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
    if (context && context->hglrc) {
        wglDeleteContext(context->hglrc);
        context->hglrc = NULL;
    }
}

thread_local WGlContext* s_lastContext = nullptr;

void WGLHelper::makeCurrent(WGlContext* context) {
    if (s_lastContext == context) {
        return;
    }

    if (context) {
        g_context_lock.lock();
        if (!wglMakeCurrent(context->hdc, context->hglrc)) {
            g_context_lock.unlock();
            CCERROR("WGL: Failed to make context[ hdc: %d | hglrc: %d] current",
                    context->hdc, context->hglrc);
        } else {
            s_lastContext = context;
            g_context_lock.unlock();
        }

    } else {
        g_context_lock.lock();
        if (!wglMakeCurrent(NULL, NULL)) {
            g_context_lock.unlock();
            CCERROR("WGL: Failed to make context empty");
        } else {
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

WGLSwapChain* WGLHelper::createWindowSurface(GL3Device& device, HWND win,
                                             uint32_t width, uint32_t height,
                                             bool singleBuffer,
                                             bool needDepthStencil,
                                             WGlContext* mainContext) {
    return new WGLSwapChain(device, win, width, height, singleBuffer,
                            needDepthStencil, mainContext);
}

void WGLHelper::deleteWindowSurface(WGLSwapChain* surface) { delete surface; }
void WGLHelper::swapBuffer(WGlContext* context) { SwapBuffers(context->hdc); }

//-----

WGLSwapChain::WGLSwapChain(GL3Device& device, HWND window, uint32_t width,
                           uint32_t height, bool singleBuffer,
                           bool needDepthStencil, WGlContext* shareContext)
    : GL3SwapChain(device, width, height, singleBuffer, needDepthStencil) {
    m_context = WGLHelper::createContext(window, shareContext, singleBuffer);
}

void WGLSwapChain::handleUpdateSurfaceInfo(const SurfaceInfo& info) {
    WGlContext* shareContext = m_context->shareContext;
    WGLHelper::deleteContext(m_context);

    m_singleBuffer = info.singleBuffer;
    m_width        = info.width;
    m_height       = info.height;

    m_context = WGLHelper::createContext((HWND)info.handle, shareContext,
                                         m_singleBuffer);

    updateAttachment(info.width, info.height);
}

void WGLSwapChain::swapBuffer() { WGLHelper::swapBuffer(m_context); }
void WGLSwapChain::makeCurrent() { WGLHelper::makeCurrent(m_context); }

void WGLSwapChain::clearCurrent() { WGLHelper::makeCurrent(nullptr); }
WGLSwapChain::~WGLSwapChain() {
    if (m_context) {
        WGLHelper::deleteContext(m_context);
    }
    m_context = nullptr;
}

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE

#endif  // #if CUR_GL_TYPE == OPENGL_WGL