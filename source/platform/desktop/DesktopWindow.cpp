#include "DesktopWindow.h"

class GlfwLoader {
public:
    GlfwLoader() { auto result = glfwInit(); }
    ~GlfwLoader() { glfwTerminate(); }
};

static void framebufferResizeCallback(GLFWwindow *win, int width, int height) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));
    // context->handle_window_resize(width, height);
}

static void mouse_callback(GLFWwindow *win, int button, int action, int mods) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));

    // context->handle_mouse_event(button, action, mods);
}
static void keyboard_callback(GLFWwindow *win, int key, int scancode,
                              int action, int mods) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));

    // context->handle_keyboard_event(key, scancode, action, mods);
}

static void cursor_pos_callback(GLFWwindow *win, double x, double y) {
    auto context =
        reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));
    // context->handle_cursor_pos_event(x, y);
}

static void cursor_enter_callback(GLFWwindow *win, int entered) {}

DesktopWindow::DesktopWindow(const std::string &title, uint32_t w, uint32_t h)
    : Window(title, w, h) {}

DesktopWindow::~DesktopWindow() { glfwDestroyWindow(m_handle); }

bool DesktopWindow::init() {
    static GlfwLoader loader;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_handle = glfwCreateWindow(m_size.width, m_size.height, m_title.c_str(),
                                nullptr, nullptr);
    glfwSetWindowUserPointer(m_handle, this);

    glfwSetFramebufferSizeCallback(m_handle, framebufferResizeCallback);
    glfwSetKeyCallback(m_handle, keyboard_callback);
    glfwSetMouseButtonCallback(m_handle, mouse_callback);
    glfwSetCursorPosCallback(m_handle, cursor_pos_callback);
    glfwSetCursorEnterCallback(m_handle, cursor_enter_callback);
    return true;
}
bool DesktopWindow::shoudleClose() { return glfwWindowShouldClose(m_handle); }
void DesktopWindow::updateEvents() { glfwPollEvents(); }