#pragma once
#include "common/Object.h"

class System : public Object {
public:
    System();
    virtual ~System();

protected:
    void update(float dt);
    virtual void onUpdate(float dt);
    virtual void onInit();
    virtual void onDestry();

protected:
};