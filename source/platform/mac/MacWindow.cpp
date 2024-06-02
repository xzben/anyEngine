#include "MacWindow.h"

#define GLFW_EXPOSE_NATIVE_NSGL
#include "GLFW/glfw3native.h"

MacWindow::MacWindow(const std::string &title, uint32_t w, uint32_t h)
    : DesktopWindow(title, w, h) {}
MacWindow::~MacWindow() {}

void *MacWindow::getNativeWinHandle() {
    NSView *nsglView = glfwGetCocoaWindow(m_handle);
    return (void *)nsglView;
}