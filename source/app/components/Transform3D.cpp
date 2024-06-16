#include "Transform3D.h"

#include "glm/ext.hpp"
BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Transform3D)

void Transform3D::updateModel() {
    if (!m_dirty) return;

    auto trans = glm::translate(glm::mat4(1.0f), m_pos);
    auto scale = glm::scale(glm::mat4(1.0f), m_scale);
    auto rotataionx =
        glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    auto rotataiony =
        glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    auto rotataionz =
        glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    m_transform = trans * rotataionx * rotataiony * rotataionz * scale;
    m_dirty     = false;
}

void Transform3D::setPosition(float x, float y, float z) { setPosition({x, y, z}); }
void Transform3D::setPosition(const glm::vec3& pos) {
    m_pos   = pos;
    m_dirty = true;
}

void Transform3D::setScale(float x, float y, float z) { setScale({x, y, z}); }
void Transform3D::setScale(float scale) { setScale({scale, scale, scale}); }
void Transform3D::setScale(const glm::vec3& scale) {
    m_scale = scale;
    m_dirty = true;
}

void Transform3D::setRotation(float x, float y, float z) { setRotation({x, y, z}); }
void Transform3D::setRotation(const glm::vec3& rotation) {
    m_rotation = rotation;
    m_dirty    = true;
}

END_NS_SCENCE_GRAPH