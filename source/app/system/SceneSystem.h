#pragma once

#include "../System.h"

BEGIN_NS_SCENCE_GRAPH
class SceneSystem : public System {
public:
    SceneSystem();
    virtual ~SceneSystem();

    virtual void onUpdate(float dt) override;
};

END_NS_SCENCE_GRAPH