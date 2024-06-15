#pragma once
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "common/Object.h"
#include "common/ObjectContainor.h"

BEGIN_NS_SCENCE_GRAPH

class Scene;
class Node : public Object, protected ObjectContainor<Component> {
    DECLARE_RUNTIME_CLASS(Node)
public:
    Node();
    virtual ~Node();

protected:
    virtual void onUpdate(float dt) {}

protected:
    void update(float dt) {
        foreach ([&](Component* com) { com->update(dt); });
        onUpdate(dt);
    }

    void enterScene(Scene* scene);
    void exitScene(Scene* scene);

    virtual void onEnterScene(Scene* scene) {}
    virtual void onExitScene(Scene* scene) {}

public:
    void addChild(Node* node);
    void removeChild(Node* node);
    void removeFromParent();
    Node* getNode(const std::string& name);
    Node* getParent();
    bool isVisible() { return m_visible; }
    void setVisible(bool visible);

public:
    template <class COM_CLASS>
    bool addComponent(const std::string& name) {
        return addObject<COM_CLASS>(name);
    }

    template <class COM_CLASS>
    COM_CLASS* getComponent() {
        return getObject<COM_CLASS>();
    }

    template <class COM_CLASS>
    COM_CLASS* getComponent(const std::string& name) {
        return getObject<COM_CLASS>(name);
    }

    void addComponent(Component* com) { addObject(com); }
    Component* getComponent(const std::string& name) { return getObject(name); }
    bool removeComponent(Component* com) { return removeObject(com); }

protected:
    void handleAddObject(Component* com) override;
    void handleRemoveObject(Component* com) override;

    virtual void handleEnableStatusChange() {}

protected:
    void enableStatusChange();

protected:
    Scene* m_curScene{nullptr};
    Node* m_parent{nullptr};
    std::vector<Node*> m_childrens;

    bool m_visible{true};
};

END_NS_SCENCE_GRAPH