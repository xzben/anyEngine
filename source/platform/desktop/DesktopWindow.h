#pragma once

#include "../Window.h"

#include <GLFW/glfw3.h>

class DesktopWindow : public Window
{
public:
    DesktopWindow(const std::string &title, uint32_t w, uint32_t h);
    virtual ~DesktopWindow();
    virtual bool init() override;
    virtual bool shoudleClose() override;
    virtual void updateEvents() override;

protected:
    GLFWwindow *m_handle{nullptr};
};