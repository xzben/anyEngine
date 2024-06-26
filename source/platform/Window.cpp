#include "Window.h"

#include "platformDefine.h"

#if CUR_PLATFORM == PLATFORM_WINDOWS
#include "win32/Win32Window.h"
#define WindowImpClass Win32Window
#elif CUR_PLATFORM == PLATFORM_MAC
#include "mac/MacWindow.h"
#define WindowImpClass MacWindow
#else
static_assert(false, "un surport platform!");
#endif

BEGIN_NS_SCENCE_GRAPH

Window *Window::create(const std::string &title, uint32_t w, uint32_t h) {
    return new WindowImpClass(title, w, h);
}

END_NS_SCENCE_GRAPH