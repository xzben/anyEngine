#include "DesktopWindow.h"

class GlfwLoader {
public:
    GlfwLoader() { auto result = glfwInit(); }
    ~GlfwLoader() { glfwTerminate(); }
};

void DesktopWindow::FramebufferResizeCallback(GLFWwindow *win, int width,
                                              int height) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));
    // context->handle_window_resize(width, height);
}

void DesktopWindow::MouseCallback(GLFWwindow *win, int button, int action,
                                  int mods) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));

    // context->handle_mouse_event(button, action, mods);
}

void DesktopWindow::KeyboardCallback(GLFWwindow *win, int key, int scancode,
                                     int action, int mods) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));

    // context->handle_keyboard_event(key, scancode, action, mods);
}

void DesktopWindow::CursorPosCallback(GLFWwindow *win, double x, double y) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));
    // context->handle_cursor_pos_event(x, y);
}

void DesktopWindow::CursorEnterCallback(GLFWwindow *win, int entered) {}

void DesktopWindow::WindowCloseCallback(GLFWwindow *win) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));
    context->handleBeforeClose();
}

DesktopWindow::DesktopWindow(const std::string &title, uint32_t w, uint32_t h)
    : Window(title, w, h) {}

DesktopWindow::~DesktopWindow() { glfwDestroyWindow(m_handle); }

bool DesktopWindow::init() {
    static GlfwLoader loader;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_handle = glfwCreateWindow(m_size.width, m_size.height, m_title.c_str(),
                                nullptr, nullptr);
    glfwSetWindowUserPointer(m_handle, this);

    glfwSetFramebufferSizeCallback(m_handle,
                                   DesktopWindow::FramebufferResizeCallback);
    glfwSetKeyCallback(m_handle, DesktopWindow::KeyboardCallback);
    glfwSetMouseButtonCallback(m_handle, DesktopWindow::MouseCallback);
    glfwSetCursorPosCallback(m_handle, DesktopWindow::CursorPosCallback);
    glfwSetCursorEnterCallback(m_handle, DesktopWindow::CursorEnterCallback);
    glfwSetWindowCloseCallback(m_handle, DesktopWindow::WindowCloseCallback);

    return true;
}

bool DesktopWindow::shoudleClose() { return glfwWindowShouldClose(m_handle); }
void DesktopWindow::updateEvents() { glfwPollEvents(); }