#pragma once

#include "platform/desktop/DesktopWindow.h"

#if CUR_PLATFORM == PLATFORM_WIN32
class Win32Window : public DesktopWindow {
public:
    Win32Window(const std::string &title, uint32_t w, uint32_t h);
    virtual ~Win32Window();

    virtual void *getNativeWinHandle() override;
};
#endif
