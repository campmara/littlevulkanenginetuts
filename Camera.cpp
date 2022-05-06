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
} // namespace XIV