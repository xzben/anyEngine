#pragma once

#include "platform/desktop/DesktopWindow.h"

BEGIN_NS_SCENCE_GRAPH

#if CUR_PLATFORM == PLATFORM_WINDOWS
class Win32Window : public DesktopWindow {
public:
    Win32Window(const std::string &title, uint32_t w, uint32_t h);
    virtual ~Win32Window();

    virtual void *getNativeWinHandle() override;
};
#endif

END_NS_SCENCE_GRAPH