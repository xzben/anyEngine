#include "DesktopWindow.h"

#include "glfwConfig.h"

class GlfwLoader {
public:
    GlfwLoader() { auto result = glfwInit(); }
    ~GlfwLoader() { glfwTerminate(); }
};

void DesktopWindow::FramebufferResizeCallback(GLFWwindow *win, int width, int height) {
    auto window = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));
    WindowResizeEvent event;
    event.width  = width;
    event.height = height;
    window->dispatchEvent(event);
}

void DesktopWindow::MouseCallback(GLFWwindow *win, int button, int action, int mods) {
    auto window = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));

    auto itButton = g_mapMouseButon.find(button);
    auto itPress  = g_pressMap.find(action);
    assert(itButton != g_mapMouseButon.end());
    assert(itPress != g_pressMap.end());

    MouseButton btn = itButton->second;
    bool preIsDown  = window->m_mouseButtonStatus[(int)btn];
    bool curIsDown  = action != GLFW_RELEASE;

    window->m_mouseButtonStatus[(int)btn] = curIsDown;

    MouseEvent mouseEvent;
    mouseEvent.button = btn;
    mouseEvent.status = itPress->second;
    window->dispatchEvent(mouseEvent);

    if (btn == MouseButton::LEFT) {
        TouchEvent event;
        event.prePos = window->m_preCursorPos;
        event.curPos = window->m_curCursorPos;

        if (preIsDown && !curIsDown) {
            event.status = TouchStatus::END;
            window->dispatchEvent(event);
        } else if (!preIsDown && curIsDown) {
            event.status = TouchStatus::BEGIN;
            window->dispatchEvent(event);
        }
    }
}

void DesktopWindow::KeyboardCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
    auto window = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));

    auto itPress = g_pressMap.find(action);
    auto it      = g_mapKeyCodeMap.find(key);

    assert(itPress != g_pressMap.end());
    assert(it != g_mapKeyCodeMap.end());

    KeyboardEvent event;
    event.code   = it->second;
    event.status = itPress->second;
    window->dispatchEvent(event);
}

void DesktopWindow::CursorPosCallback(GLFWwindow *win, double x, double y) {
    auto window = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));
    // context->handle_cursor_pos_event(x, y);

    window->m_preCursorPos = window->m_curCursorPos;
    window->m_curCursorPos = {(float)x, (float)y};

    if (window->m_mouseButtonStatus[(int)MouseButton::LEFT]) {
        TouchEvent event;
        event.status = TouchStatus::MOVE;
        event.curPos = window->m_curCursorPos;
        event.prePos = window->m_preCursorPos;
        window->dispatchEvent(event);
    }
}

void DesktopWindow::CursorEnterCallback(GLFWwindow *win, int entered) {
    auto window = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));

    if (entered) {
        Event event(EventType::CURSOR_ENTER);
        window->dispatchEvent(event);
    } else {
        Event event(EventType::CURSOR_EXIT);
        window->dispatchEvent(event);
    }
}

void DesktopWindow::WindowCloseCallback(GLFWwindow *win) {
    auto window = reinterpret_cast<DesktopWindow *>(glfwGetWindowUserPointer(win));
    Event event(EventType::WINDOW_BEFORE_CLOSE);
    window->dispatchEvent(event);
}

DesktopWindow::DesktopWindow(const std::string &title, uint32_t w, uint32_t h)
    : Window(title, w, h) {}

DesktopWindow::~DesktopWindow() { glfwDestroyWindow(m_handle); }

bool DesktopWindow::init() {
    static GlfwLoader loader;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    m_handle = glfwCreateWindow(m_size.width, m_size.height, m_title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_handle, this);

    glfwSetFramebufferSizeCallback(m_handle, DesktopWindow::FramebufferResizeCallback);
    glfwSetKeyCallback(m_handle, DesktopWindow::KeyboardCallback);
    glfwSetMouseButtonCallback(m_handle, DesktopWindow::MouseCallback);
    glfwSetCursorPosCallback(m_handle, DesktopWindow::CursorPosCallback);
    glfwSetCursorEnterCallback(m_handle, DesktopWindow::CursorEnterCallback);
    glfwSetWindowCloseCallback(m_handle, DesktopWindow::WindowCloseCallback);

    return true;
}

bool DesktopWindow::shoudleClose() { return glfwWindowShouldClose(m_handle); }
void DesktopWindow::updateEvents() { glfwPollEvents(); }