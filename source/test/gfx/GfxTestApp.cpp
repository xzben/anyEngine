#include "GfxTestApp.h"

#include "GfxRenderSystem.h"

GfxTestApp::GfxTestApp(Window* window) : Application("GfxTestApp", window) {
    auto system = addSystem<GfxRenderSystem>("gfxRenderSystem", window);
}

GfxTestApp::~GfxTestApp() {}

void GfxTestApp::onUpdate(float dt) {}
void GfxTestApp::onUpdateWindow(Window* m_window) {}