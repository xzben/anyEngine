#include "Node.h"

#include "common.h"
BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Node)
Node::Node() {}
Node::~Node() {}
void Node::handleAddObject(Component* com) { com->setParent(this); }
void Node::handleRemoveObject(Component* com) {
    CCASSERT(com->getParent() == this, "com parent must been this node ");
    com->setParent(nullptr);
}

void Node::setVisible(bool visible) {
    if (m_visible == visible) return;
    m_visible = visible;

    enableStatusChange();
}

void Node::enableStatusChange() {
    foreach ([](Component* com) { com->enableStatusChange(); });
    handleEnableStatusChange();
}

void Node::enterScene(Scene* scene) {
    if (m_curScene != nullptr) {
        exitScene(m_curScene);
        m_curScene = nullptr;
    }
    m_curScene = scene;
    foreach ([=](Component* com) { com->enterScene(scene); });
    onEnterScene(scene);
}

void Node::exitScene(Scene* scene) {
    CCASSERT(m_curScene == scene, "exit scene must equal");
    m_curScene = nullptr;
    foreach ([=](Component* com) { com->exitScene(scene); });
    onExitScene(scene);
}

END_NS_SCENCE_GRAPH