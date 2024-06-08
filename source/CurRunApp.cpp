#include "CurRunApp.h"

#if defined(USE_GFX_TEST)
#include "test/gfx/GfxTestApp.h"
using CurRunApp = GfxTestApp;
#else
static_assert("please set run app");
#endif

Application* createRunApp(const char* appName, uint32_t width, uint32_t height) {
    Window* win = Window::create(appName, width, height);
    win->init();

    Application* app = new CurRunApp(win);
    win->addEventListener(EventType::WINDOW_BEFORE_CLOSE, [&](const Event& event) { app->exit(); });
    RELEASE_OBJ(win);

    return app;
}