#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "System.h"
#include "common/Object.h"
#include "common/ObjectContainor.h"

class Application : public Object, protected ObjectContainor<System> {
public:
    Application(const std::string& name);
    virtual ~Application();

    void run();
    void exit();

public:
    template <class SYS_CLASS>
    bool addSystem(const std::string& name) {
        return addObject<SYS_CLASS>(name);
    }

    template <class SYS_CLASS>
    SYS_CLASS* getSystem() {
        return getObject<SYS_CLASS>();
    }

    template <class SYS_CLASS>
    SYS_CLASS* getSystem(const std::string& name) {
        return getObject<SYS_CLASS>(name);
    }

    void addSystem(System* com) { addObject(com); }
    System* getSystem(const std::string& name) { getObject(name); }

public:
    virtual void onInit();
    virtual void onDestroy();
    virtual void onUpdate(float dt);

protected:
    float m_fps{1.0 / 60};
    bool m_exit{false};
};