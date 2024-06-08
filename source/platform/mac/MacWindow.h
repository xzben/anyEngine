#pragma once

#include "platform/desktop/DesktopWindow.h"

BEGIN_NS_SCENCE_GRAPH
#if CUR_PLATFORM == PLATFORM_MAC
class MacWindow : public DesktopWindow {
public:
    MacWindow(const std::string &title, uint32_t w, uint32_t h);
    virtual ~MacWindow();

    virtual void *getNativeWinHandle() override;
};

#endif  // #if CUR_PLATFORM == PLATFORM_MAC

END_NS_SCENCE_GRAPH