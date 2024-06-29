#pragma once

#include "glm/glm.hpp"
#include "pipelineMacro.h"
BEGIN_PIPELINE_NAMESPACE

struct Plane {
    glm::vec3 normal;
    float distance;
};

class Frustum {
public:
    Frustum(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
    void update(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
    bool isPointInside(const glm::vec3& point) const;
    bool isSphereInside(const glm::vec3& center, float radius) const;
    bool isAABBInside(const glm::vec3& min, const glm::vec3& max) const;
    bool isOBBInside(const glm::vec3& center, const glm::vec3& axis0, const glm::vec3& axis1,
                     const glm::vec3& axis2, const glm::vec3& halfSize) const;

private:
    Plane planes[6];
};

END_PIPELINE_NAMESPACE