#pragma once

#include "../System.h"

BEGIN_NS_SCENCE_GRAPH
class SceneSystem : public System {
    DECLARE_RUNTIME_CLASS(SceneSystem)
public:
    SceneSystem();
    virtual ~SceneSystem();

    virtual void onUpdate(float dt) override;
    virtual void onInit() override;
    virtual void onUnInit() override;
};

END_NS_SCENCE_GRAPH