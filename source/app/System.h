#pragma once
#include "common/Object.h"

BEGIN_NS_SCENCE_GRAPH
class System : public Object {
    friend class Application;

public:
    System()          = default;
    virtual ~System() = default;

protected:
    virtual void onUpdate(float dt) = 0;

protected:
    void update(float dt) { onUpdate(dt); }

protected:
};

END_NS_SCENCE_GRAPH