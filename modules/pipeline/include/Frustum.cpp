#include "Frustum.h"

BEGIN_PIPELINE_NAMESPACE

Frustum::Frustum() {}

Frustum::~Frustum() {}

void Frustum::update(const glm::mat4& view, const glm::mat4& projection) {
    m_planes.clear();

    // Left plane
    m_planes[0].normal.x = vp[0][3] + vp[0][0];
    m_planes[0].normal.y = vp[1][3] + vp[1][0];
    m_planes[0].normal.z = vp[2][3] + vp[2][0];
    m_planes[0].distance = vp[3][3] + vp[3][0];

    // Right plane
    m_planes[1].normal.x = vp[0][3] - vp[0][0];
    m_planes[1].normal.y = vp[1][3] - vp[1][0];
    m_planes[1].normal.z = vp[2][3] - vp[2][0];
    m_planes[1].distance = vp[3][3] - vp[3][0];

    // Top plane
    m_planes[2].normal.x = vp[0][3] - vp[0][1];
    m_planes[2].normal.y = vp[1][3] - vp[1][1];
    m_planes[2].normal.z = vp[2][3] - vp[2][1];
    m_planes[2].distance = vp[3][3] - vp[3][1];

    // Bottom plane
    m_planes[3].normal.x = vp[0][3] + vp[0][1];
    m_planes[3].normal.y = vp[1][3] + vp[1][1];
    m_planes[3].normal.z = vp[2][3] + vp[2][1];
    m_planes[3].distance = vp[3][3] + vp[3][1];

    // Near plane
    m_planes[4].normal.x = vp[0][3] + vp[0][2];
    m_planes[4].normal.y = vp[1][3] + vp[1][2];
    m_planes[4].normal.z = vp[2][3] + vp[2][2];
    m_planes[4].distance = vp[3][3] + vp[3][2];

    // Far plane
    m_planes[5].normal.x = vp[0][3] - vp[0][2];
    m_planes[5].normal.y = vp[1][3] - vp[1][2];
    m_planes[5].normal.z = vp[2][3] - vp[2][2];
    m_planes[5].distance = vp[3][3] - vp[3][2];

    // Normalize the planes
    for (int i = 0; i < 6; i++) {
        float length = glm::length(m_planes[i].normal);
        m_planes[i].normal /= length;
        m_planes[i].distance /= length;
    }
}

bool Frustum::isPointInside(const glm::vec3& point) const {
    for (int i = 0; i < 6; i++) {
        if (glm::dot(m_planes[i].normal, point) + m_planes[i].distance < 0) {
            return false;
        }
    }
    return true;
}
bool Frustum::isSphereInside(const glm::vec3& center, float radius) const {
    for (int i = 0; i < 6; i++) {
        float distance = glm::dot(m_planes[i].normal, center) + m_planes[i].distance;
        if (distance < -radius) {
            return false;
        }
        if (distance < radius) {
            return true;
        }
    }
    return true;
}
bool Frustum::isAABBInside(const glm::vec3& min, const glm::vec3& max) const {
    glm::vec3 corners[8];
    corners[0] = min;

    corners[1].x = max.x;
    corners[1].y = min.y;
    corners[1].z = min.z;
    corners[2].x = min.x;
    corners[2].y = max.y;
    corners[2].z = min.z;
    corners[3].x = max.x;
    corners[3].y = max.y;
    corners[3].z = min.z;

    corners[4].x = min.x;
    corners[4].y = min.y;
    corners[4].z = max.z;
    corners[5].x = max.x;
    corners[5].y = max.y;
    corners[5].z = max.z;
    corners[6].x = min.x;
    corners[6].y = min.y;
    corners[6].z = min.z;
    corners[7].x = max.x;
    corners[7].y = max.y;
    corners[7].z = max.z;

    for (int i = 0; i < 6; i++) {
        float distance = glm::dot(m_planes[i].normal, corners[0]) + m_planes[i].distance;
    }
    return true;
}

// Helper function to compute the projection radius of the OBB onto a plane normal
float computeProjectionRadius(const glm::vec3& axis0, const glm::vec3& axis1,
                              const glm::vec3& axis2, const glm::vec3& halfSize,
                              const glm::vec3& planeNormal) {
    return halfSize.x * glm::abs(glm::dot(planeNormal, axis0))
           + halfSize.y * glm::abs(glm::dot(planeNormal, axis1))
           + halfSize.z * glm::abs(glm::dot(planeNormal, axis2));
}

bool Frustum::isOBBInside(const glm::vec3& center, const glm::vec3& axis0, const glm::vec3& axis1,
                          const glm::vec3& axis2, const glm::vec3& halfSize) const {
    for (int i = 0; i < 6; i++) {
        {
            const auto& planeNormal = m_planes[i];

            float planeDistance = plane.distance;

            // Compute the projection radius of the OBB onto the plane normal
            float projectionRadius =
                computeProjectionRadius(axis0, axis1, axis2, halfSize, planeNormal);

            // Compute the distance from the OBB center to the plane
            float distance = glm::dot(planeNormal, center) + planeDistance;

            // If the distance is less than the negative projection radius, the OBB is outside this
            // plane
            if (distance < -projectionRadius) {
                return false;
            }
        }

        // If the OBB is not outside any of the planes, it is inside the frustum
        return true;
    }

    END_PIPELINE_NAMESPACE