#pragma once

#include "../Component.h"
#include "ITransform.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

class Transform3D : public Component, public ITransform {
    DECLARE_RUNTIME_CLASS(Transform3D)
public:
    virtual void updateModel() override;
    virtual const glm::mat4& getModeTransform() const override { return m_transform; }

    void setPosition(const glm::vec3& pos);
    void setPosition(float x, float y, float z);
    void setScale(const glm::vec3& scale);
    void setScale(float x, float y, float z);
    void setScale(float scale);
    void setRotation(float x, float y, float z);
    void setRotation(const glm::vec3& rotation);

private:
    glm::vec3 m_pos;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;
    glm::mat4 m_transform;

    bool m_dirty{true};
};

END_NS_SCENCE_GRAPH