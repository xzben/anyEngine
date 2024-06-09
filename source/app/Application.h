#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "System.h"
#include "common/Object.h"
#include "common/ObjectContainor.h"
#include "platform/Window.h"
#include "system/RenderSystem.h"
#include "system/SceneSystem.h"

BEGIN_NS_SCENCE_GRAPH

class RenderSystem;

class Application : public Object, protected ObjectContainor<System> {
    DECLARE_RUNTIME_CLASS(Application)
public:
    static Application* getInstance();

public:
    Application(const std::string& name, Window* window);
    virtual ~Application();

    void setWindow(Window* window);
    bool init();

    void run();
    void exit();

    RenderSystem* getRenderSystem() { return m_renderSystem; }
    SceneSystem* getSceneSystem() { return m_sceneSystem; }

protected:
    void unInit();

protected:
    virtual void onUpdate(float dt) {}
    virtual void onUpdateWindow(Window* m_window) {}
    virtual bool onInit();
    virtual void onUnInit();

public:
    template <class SYS_CLASS, typename... Args>
    SYS_CLASS* addSystem(const std::string& name, Args&... args) {
        return addObject<SYS_CLASS>(name, args...);
    }

    template <class SYS_CLASS>
    SYS_CLASS* getSystem() {
        return getObject<SYS_CLASS>();
    }

    template <class SYS_CLASS>
    SYS_CLASS* getSystem(const std::string& name) {
        return getObject<SYS_CLASS>(name);
    }

    bool addSystem(System* com) { return addObject(com); }
    System* getSystem(const std::string& name) { getObject(name); }

protected:
    void update(float dt);

    virtual void handleAddObject(System* sys) override;
    virtual void handleRemoveObject(System* sys) override;

protected:
    time_t m_lasttime{0};
    float m_gameRunTime{0};
    uint32_t m_frameCount{0};
    float m_fps{1.0f / 60.f};
    bool m_exit{false};
    std::string m_name;
    Window* m_window{nullptr};
    RenderSystem* m_renderSystem{nullptr};
    SceneSystem* m_sceneSystem{nullptr};
};

END_NS_SCENCE_GRAPH