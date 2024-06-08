#include "GfxTestApp.h"

#include "GfxRenderSystem.h"

BEGIN_NS_SCENCE_GRAPH
GfxTestApp::GfxTestApp(Window* window) : Application("GfxTestApp", window) {
    auto system = addSystem<GfxRenderSystem>("gfxRenderSystem", window);
}

GfxTestApp::~GfxTestApp() {}

void GfxTestApp::onUpdate(float dt) {}
void GfxTestApp::onUpdateWindow(Window* m_window) {}

END_NS_SCENCE_GRAPH