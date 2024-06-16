#include "Node.h"

#include "app/components/Transform2D.h"
#include "app/components/Transform3D.h"
#include "common.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Node)
Node::Node() {}
Node::~Node() {}

void Node::clearTransformCom() {
    if (m_uiTransform != nullptr) {
        auto transform = m_uiTransform;
        m_uiTransform  = nullptr;
        this->removeComponent(transform);
    }

    if (m_transform != nullptr) {
        auto transform = m_transform;
        m_transform    = nullptr;
        this->removeComponent(transform);
    }
}

void Node::handleAddObject(Component* com) {
    com->setParent(this);

    if (com->getRuntime() == Transform2D::GetRuntime()) {
        clearTransformCom();
        m_uiTransform = com->toCast<Transform2D>();
    } else if (com->GetRuntime() == Transform3D::GetRuntime()) {
        clearTransformCom();
        m_transform = com->toCast<Transform3D>();
    }
}

void Node::handleRemoveObject(Component* com) {
    CCASSERT(com->getParent() == this, "com parent must been this node ");
    com->setParent(nullptr);

    if (com == m_uiTransform) {
        m_uiTransform = nullptr;
    } else if (com == m_transform) {
        m_transform = nullptr;
    }
}

void Node::addChild(Node* node) {
    CCASSERT(node != nullptr, "add node must not been nullptr");
    node->removeFromParent();

    m_childrens.push_back(node);
    node->m_parent = this;
}

void Node::removeChild(Node* node) {
    for (auto it = m_childrens.begin(); it != m_childrens.end(); it++) {
        if (*it == node) {
            it = m_childrens.erase(it);
            break;
        }
    }
}

void Node::removeFromParent() {
    if (m_parent == nullptr) {
        return;
    }

    m_parent->removeChild(this);
}

Node* Node::getChildByName(const std::string& name) {
    for (auto it = m_childrens.begin(); it != m_childrens.end(); it++) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }

    return nullptr;
}

Node* Node::getParent() { return m_parent; }

bool Node::isVisible() { return m_visible; }

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