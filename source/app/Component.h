#pragma once
#include "common/Object.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

class Component : public Object {
    friend class Node;

protected:
    virtual void onUpdate(float dt) {}

protected:
    void update(float dt) { onUpdate(dt); }

public:
    Component(const std::string& name) : Object(name) {}
    virtual ~Component() {}

protected:
};

END_NS_SCENCE_GRAPH