#pragma once
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "common/Object.h"
#include "common/ObjectContainor.h"

BEGIN_NS_SCENCE_GRAPH
class Node : public Object, protected ObjectContainor<Component> {
    DECLARE_RUNTIME_CLASS(Node)
public:
    Node();
    virtual ~Node();

protected:
    virtual void onUpdate(float dt);

protected:
    void update(float dt) {
        foreach ([&](Component* com) { com->update(dt); });
        onUpdate(dt);
    }

public:
    void addChild(Node* node);
    void removeChild(Node* node);
    void removeFromParent();
    Node* getNode(const std::string& name);
    Node* getParent();

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
    Component* getComponent(const std::string& name) { getObject(name); }

protected:
    Node* m_parent{nullptr};
    std::vector<Node*> m_childrens;
};

END_NS_SCENCE_GRAPH