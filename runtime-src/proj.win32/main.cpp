#include <iostream>

#include "platform/Window.h"
#include "test/gfx/GfxTestApp.h"

int main() {
    Window* win = Window::create("test", 800, 800);
    win->init();

    GfxTestApp app(win);
    win->setBeforeCloseCallback([&](Window* win) { app.exit(); });
    app.run();

    RELEASE_OBJ(win);

    return 0;
}