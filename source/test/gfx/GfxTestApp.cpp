#include "GfxTestApp.h"

#include "GfxRenderSystem.h"

BEGIN_NS_SCENCE_GRAPH
IMPLEMENT_RUNTIME_CLASS(GfxTestApp)

GfxTestApp::GfxTestApp(Window* window) : Application("GfxTestApp", window) {}

GfxTestApp::~GfxTestApp() {}

bool GfxTestApp::onInit() {
    addSystem<GfxRenderSystem>("RenderSystem", m_window);
    addSystem<SceneSystem>("SceneSystem");

    return true;
}

void GfxTestApp::onUpdate(float dt) {}
void GfxTestApp::onUpdateWindow(Window* m_window) {}

END_NS_SCENCE_GRAPH