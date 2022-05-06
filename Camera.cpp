#include "Camera.h"

#include <cassert>
#include <limits>

namespace XIV {
    void Camera::SetOrthographicProjection(
        float left, float right, float top, float bottom, float near, float far) {
        ProjectionMatrix = glm::mat4{1.0f};
        ProjectionMatrix[0][0] = 2.f / (right - left);
        ProjectionMatrix[1][1] = 2.f / (bottom - top);
        ProjectionMatrix[2][2] = 1.f / (far - near);
        ProjectionMatrix[3][0] = -(right + left) / (right - left);
        ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        ProjectionMatrix[3][2] = -near / (far - near);
    }

    void Camera::SetPerspectiveProjection(float fovY, float aspect, float near, float far) {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovy = tan(fovY / 2.f);
        ProjectionMatrix = glm::mat4{0.0f};
        ProjectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
        ProjectionMatrix[1][1] = 1.f / (tanHalfFovy);
        ProjectionMatrix[2][2] = far / (far - near);
        ProjectionMatrix[2][3] = 1.f;
        ProjectionMatrix[3][2] = -(far * near) / (far - near);
    }

    void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};

        ViewMatrix = glm::mat4{1.0f};
        ViewMatrix[0][0] = u.x;
        ViewMatrix[1][0] = u.y;
        ViewMatrix[2][0] = u.z;
        ViewMatrix[0][1] = v.x;
        ViewMatrix[1][1] = v.y;
        ViewMatrix[2][1] = v.z;
        ViewMatrix[0][2] = w.x;
        ViewMatrix[1][2] = w.y;
        ViewMatrix[2][2] = w.z;
        ViewMatrix[3][0] = -glm::dot(u, position);
        ViewMatrix[3][1] = -glm::dot(v, position);
        ViewMatrix[3][2] = -glm::dot(w, position);
    }

    void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        SetViewDirection(position, target - position, up);
    }

    void Camera::SetViewXYZ(glm::vec3 position, glm::vec3 rotation) {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
        const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
        const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
        ViewMatrix = glm::mat4{1.f};
        ViewMatrix[0][0] = u.x;
        ViewMatrix[1][0] = u.y;
        ViewMatrix[2][0] = u.z;
        ViewMatrix[0][1] = v.x;
        ViewMatrix[1][1] = v.y;
        ViewMatrix[2][1] = v.z;
        ViewMatrix[0][2] = w.x;
        ViewMatrix[1][2] = w.y;
        ViewMatrix[2][2] = w.z;
        ViewMatrix[3][0] = -glm::dot(u, position);
        ViewMatrix[3][1] = -glm::dot(v, position);
        ViewMatrix[3][2] = -glm::dot(w, position);
    }
} // namespace XIV