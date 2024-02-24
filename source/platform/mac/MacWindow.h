#pragma once

#include "../desktop/DesktopWindow.h"

class MacWindow : public DesktopWindow
{
public:
    MacWindow(const std::string &title, uint32_t w, uint32_t h);
    virtual ~MacWindow();
};