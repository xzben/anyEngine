#pragma once
#include "common/Object.h"

class Component : public Object {
protected:
    virtual void onUpdate(float dt);
    virtual void onInit();
    virtual void onDestroy();

public:
    Component(const std::string& name) : Object(name) {}
    virtual ~Component() {}

protected:
};