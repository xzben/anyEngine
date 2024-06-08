#pragma once

#include <GLFW/glfw3.h>

#include "../Window.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH
class DesktopWindow : public Window {
protected:
    static void FramebufferResizeCallback(GLFWwindow *win, int width, int height);
    static void WindowCloseCallback(GLFWwindow *win);
    static void KeyboardCallback(GLFWwindow *win, int key, int scancode, int action, int mods);
    static void CursorPosCallback(GLFWwindow *win, double x, double y);
    static void CursorEnterCallback(GLFWwindow *win, int entered);
    static void MouseCallback(GLFWwindow *win, int button, int action, int mods);

public:
    DesktopWindow(const std::string &title, uint32_t w, uint32_t h);
    virtual ~DesktopWindow();
    virtual bool init() override;
    virtual bool shoudleClose() override;
    virtual void updateEvents() override;

protected:
    GLFWwindow *m_handle{nullptr};

    bool m_mouseButtonStatus[(int)MouseButton::Count];
    Position m_curCursorPos{0.f, 0.f};
    Position m_preCursorPos{0.f, 0.f};
};
END_NS_SCENCE_GRAPH