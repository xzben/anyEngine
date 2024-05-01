#pragma once

#include "../System.h"

class SceneSystem : public System {
public:
    SceneSystem();
    virtual ~SceneSystem();

    virtual void onUpdate(float dt) override;
};