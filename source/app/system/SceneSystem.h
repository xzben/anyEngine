#pragma once

#include "../System.h"
#include "app/Node.h"

BEGIN_NS_SCENCE_GRAPH

class Node;

class SceneSystem : public System {
    DECLARE_RUNTIME_CLASS(SceneSystem)
public:
    SceneSystem();
    virtual ~SceneSystem();

    virtual void onUpdate(float dt) override;
    virtual void onInit() override;
    virtual void onUnInit() override;

    Node& getRootNode() { return m_rootNode; }
    Node& getPersistentNode() { return m_persistentNode; }

protected:
    Node m_rootNode;
    Node m_persistentNode;
};

END_NS_SCENCE_GRAPH