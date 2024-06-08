#include "Win32Window.h"

BEGIN_NS_SCENCE_GRAPH

#if CUR_PLATFORM == PLATFORM_WINDOWS

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

Win32Window::Win32Window(const std::string &title, uint32_t w, uint32_t h)
    : DesktopWindow(title, w, h) {}
Win32Window::~Win32Window() {}

void *Win32Window::getNativeWinHandle() {
    HWND hwnd = glfwGetWin32Window(m_handle);
    return (void *)hwnd;
}

#endif

END_NS_SCENCE_GRAPH