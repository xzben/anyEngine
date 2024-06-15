#pragma once
#include "common/Object.h"

BEGIN_NS_SCENCE_GRAPH
class System : public Object {
    DECLARE_RUNTIME_CLASS(System)
public:
    friend class Application;
    System()          = default;
    virtual ~System() = default;

protected:
    virtual void onUpdate(float dt) = 0;
    virtual void onInit()           = 0;
    virtual void onUnInit()         = 0;

protected:
    virtual void update(float dt) { onUpdate(dt); }
    virtual void init() { onInit(); }
    virtual void unInit() { onUnInit(); }
};

END_NS_SCENCE_GRAPH