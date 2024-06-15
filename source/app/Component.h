#pragma once
#include "common/Object.h"
#include "rtti.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH
class Scene;
class Component : public Object {
    DECLARE_RUNTIME_CLASS(Component)
public:
    friend class Node;

protected:
    virtual void onUpdate(float dt) {}

protected:
    void update(float dt) { onUpdate(dt); }
    void setParent(Node* node);
    void removeFromParent();
    void enableStatusChange();

    void enterScene(Scene* scene);
    void exitScene(Scene* scene);

    virtual void onEnterScene(Scene* scene) {}
    virtual void onExitScene(Scene* scene) {}

public:
    Component() = default;
    Component(const std::string& name) : Object(name) {}
    virtual ~Component() = default;

    Node* getParent() { return m_parentNode; }
    void setEnable(bool enable);
    bool isEnable();

protected:
    virtual void handleEnableStatusChange() {}

protected:
    bool m_enable{true};
    Node* m_parentNode{nullptr};
};

END_NS_SCENCE_GRAPH