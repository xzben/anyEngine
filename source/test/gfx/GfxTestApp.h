#pragma once

#include "app/Application.h"

class GfxTestApp : public Application {
public:
    GfxTestApp(Window* window);
    virtual ~GfxTestApp();

protected:
    virtual void onUpdate(float dt) override;
    virtual void onUpdateWindow(Window* m_window) override;
};