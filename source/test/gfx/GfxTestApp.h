#pragma once

#include "app/Application.h"

BEGIN_NS_SCENCE_GRAPH
class GfxTestApp : public Application {
    DECLARE_RUNTIME_CLASS(GfxTestApp)
public:
    GfxTestApp(Window* window);
    virtual ~GfxTestApp();

protected:
    bool onInit() override;

    virtual void onUpdate(float dt) override;
    virtual void onUpdateWindow(Window* m_window) override;
};

END_NS_SCENCE_GRAPH