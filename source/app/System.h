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
    void update(float dt) { onUpdate(dt); }
    void init() { onInit(); }
    void unInit() { onUnInit(); }
};

END_NS_SCENCE_GRAPH