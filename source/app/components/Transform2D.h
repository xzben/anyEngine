#pragma once

#include "../Component.h"
#include "ITransform.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH
class Transform2D : public Component, public ITransform {
    DECLARE_RUNTIME_CLASS(Transform2D)
public:
    virtual void updateModel() override;
    virtual const glm::mat4& getModeTransform() const override { return m_transform; }

    void setPosition(const glm::vec2& pos);
    void setPosition(float x, float y);
    void setScale(const glm::vec2& scale);
    void setScale(float x, float y);
    void setScale(float scale);
    void setRotation(float rotation);

private:
    glm::vec2 m_pos;
    glm::vec2 m_scale;
    float m_rotation;

    glm::mat4 m_transform;
    bool m_dirty{true};
};

END_NS_SCENCE_GRAPH