#include "wgl.h"

#include <mutex>

#if CUR_GL_TYPE == OPENGL_WGL

BEGIN_GFX_NAMESPACE

bool WGLHelper::init() {
    HDC hdc         = ::GetDC(NULL);
    HGLRC tempHglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempHglrc);
    glewInit();
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempHglrc);

    return true;
}

static std::mutex g_context_lock;
WGlContext* WGLHelper::createContext(HWND window, WGlContext* share) {
    WGlContext* context = new WGlContext();
    bool success        = true;
    g_context_lock.lock();

    {
        HDC hdc = NULL;
        if (window == NULL) {
            hdc              = ::GetDC(NULL);
            context->selfHdc = true;
        } else {
            hdc = ::GetDC(window);
        }

        HGLRC shareHglrc = share == nullptr ? NULL : share->hglrc;
        if (WGL_ARB_create_context) {
            int attribs[40];
            context->hglrc =
                wglCreateContextAttribsARB(hdc, shareHglrc, attribs);
        } else {
            context->hglrc = wglCreateContext(hdc);

            if (shareHglrc) {
                if (!wglShareLists(context->hglrc, shareHglrc)) {
                    success = false;
                }
            }
        }
    }

    g_context_lock.unlock();

    assert(success);

    return context;
}

void WGLHelper::deleteContext(WGlContext* context) {}
WGLSwapChain* WGLHelper::createWindowSurface(void* win, uint32_t width,
                                             uint32_t height) {
    return nullptr;
}
void WGLHelper::makeCurrent(WGlContext* context, WGLSwapChain* surface) {}
void WGLHelper::deleteWindowSurface(WGLSwapChain* surface) {}
void WGLHelper::swapBuffer(WGlContext* context) {}

END_GFX_NAMESPACE

#endif  // #if CUR_GL_TYPE == OPENGL_WGL