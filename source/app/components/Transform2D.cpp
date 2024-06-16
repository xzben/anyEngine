#include "Transform2D.h"

#include "glm/ext.hpp"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Transform2D)

void Transform2D::updateModel() {
    if (!m_dirty) return;

    glm::mat4 trans;
    trans       = glm::translate(trans, glm::vec3(m_pos.x, m_pos.y, 0.0));
    trans       = glm::rotate(trans, m_rotation, glm::vec3(0.0, 0.0, 1.0));
    trans       = glm::scale(trans, glm::vec3(m_scale.x, m_scale.y, 1.0));
    m_transform = trans;
    m_dirty     = false;
}

void Transform2D::setPosition(float x, float y) { setPosition({x, y}); }
void Transform2D::setPosition(const glm::vec2& pos) {
    if (m_pos == pos) return;
    m_pos   = pos;
    m_dirty = true;
}

void Transform2D::setScale(float scale) { setScale({scale, scale}); }
void Transform2D::setScale(float x, float y) { setScale({x, y}); }
void Transform2D::setScale(const glm::vec2& scale) {
    if (m_scale == scale) return;
    m_scale = scale;
    m_dirty = true;
}

void Transform2D::setRotation(float rotation) {
    m_rotation = rotation;
    m_dirty    = true;
}

END_NS_SCENCE_GRAPH