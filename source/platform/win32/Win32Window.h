#pragma once

#include "../Window.h"

#include <GLFW/glfw3.h>

class Win32Window : public Window
{
public:
    Win32Window(const std::string &title, uint32_t w, uint32_t h);
    virtual ~Win32Window();
    virtual bool init() override;
    virtual bool shoudleClose() override;
    virtual void updateEvents() override;

protected:
private:
    GLFWwindow *m_handle{nullptr};
};