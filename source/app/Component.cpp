#include "Component.h"

#include "Node.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Component)

void Component::setParent(Node* node) {
    removeFromParent();
    m_parentNode = node;
}

void Component::removeFromParent() {
    if (m_parentNode) {
        m_parentNode->removeComponent(this);
        m_parentNode = nullptr;
    }
}

void Component::setEnable(bool enable) {
    if (m_enable == enable) return;
    m_enable = enable;
}

bool Component::isEnable() {
    if (m_parentNode == nullptr || !m_parentNode->isVisible()) {
        return false;
    }

    return m_enable;
}

void Component::enableStatusChange() { handleEnableStatusChange(); }

void Component::enterScene(Scene* scene) { onEnterScene(scene); }

void Component::exitScene(Scene* scene) { onExitScene(scene); }

END_NS_SCENCE_GRAPH