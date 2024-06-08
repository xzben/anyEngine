#pragma once

#include "platform/desktop/DesktopWindow.h"

#if CUR_PLATFORM == PLATFORM_MAC
class MacWindow : public DesktopWindow {
public:
    MacWindow(const std::string &title, uint32_t w, uint32_t h);
    virtual ~MacWindow();

    virtual void *getNativeWinHandle() override;
};

#endif  // #if CUR_PLATFORM == PLATFORM_MAC