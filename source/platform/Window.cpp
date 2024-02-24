#include "Window.h"

#if CUR_PLATFORM == PLATFORM_WINDOWS
#include "win32/Win32Window.h"
#define WindowImpClass Win32Window
#else
static_assert(false, "un surport platform!");
#endif

Window *Window::create(const std::string &title, uint32_t w, uint32_t h)
{
    return new WindowImpClass(title, w, h);
}