#include "GLContext.h"

#include "../GL3Device.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

GLContext::GLContext(GL3Device& device) : m_device(device) {
#if CUR_GL_TYPE == OPENGL_WGL
    m_context = WGLHelper::createContext(NULL, nullptr, true);
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}
GLContext::~GLContext() {}

GL3SwapChain* GLContext::createSwapChain(void* window, uint32_t width,
                                         uint32_t height, bool singleBuffer) {
#if CUR_GL_TYPE == OPENGL_WGL
    return WGLHelper::createWindowSurface(m_device, (HWND)window, width, height,
                                          singleBuffer, m_context);
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
    return nullptr;
#endif
}
void GLContext::destroySwapChain(GL3SwapChain* swapChain) {
#if CUR_GL_TYPE == OPENGL_WGL
    WGLHelper::deleteWindowSurface(dynamic_cast<WGLSwapChain*>(swapChain));
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}
void GLContext::swapBuffer(GL3SwapChain* swapChain) {
#if CUR_GL_TYPE == OPENGL_WGL
    dynamic_cast<WGLSwapChain*>(swapChain)->swapBuffer();
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}

void GLContext::makeCurrent(GL3SwapChain* swapChain) {
#if CUR_GL_TYPE == OPENGL_WGL
    if (swapChain == nullptr) {
        WGLHelper::makeCurrent(m_context);
    } else {
        dynamic_cast<WGLSwapChain*>(swapChain)->makeCurrent();
    }
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE