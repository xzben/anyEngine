#include "CurRunApp.h"

#if defined(USE_GFX_TEST)
#include "test/gfx/GfxTestApp.h"
using CurRunApp = NS_SG::GfxTestApp;
#else
static_assert("please set run app");
#endif

BEGIN_NS_SCENCE_GRAPH

Application* createRunApp(const char* appName, uint32_t width, uint32_t height) {
    Window* win = Window::create(appName, width, height);
    win->init();

    Application* app = new CurRunApp(win);
    app->init();
    RELEASE_OBJ(win);

    return app;
}
END_NS_SCENCE_GRAPH