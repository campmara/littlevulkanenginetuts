#include "camera.h"

#include <cassert>
#include <limits>

namespace XIV {
    void Camera::SetOrthographicProjection(
        float left, float right, float top, float bottom, float near, float far) {
        ProjectionMatrix = Mat4{1.0f};
        ProjectionMatrix[0][0] = 2.f / (right - left);
        ProjectionMatrix[1][1] = 2.f / (bottom - top);
        ProjectionMatrix[2][2] = 1.f / (far - near);
        ProjectionMatrix[3][0] = -(right + left) / (right - left);
        ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        ProjectionMatrix[3][2] = -near / (far - near);
    }

    void Camera::SetPerspectiveProjection(float fovY, float aspect, float near, float far) {
        assert(Wrath::Abs(aspect - Wrath::Epsilon()) > 0.0f);
        const float tanHalfFovy = tan(fovY / 2.0f);
        ProjectionMatrix = Mat4{0.0f};
        ProjectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
        ProjectionMatrix[1][1] = 1.f / (tanHalfFovy);
        ProjectionMatrix[2][2] = far / (far - near);
        ProjectionMatrix[2][3] = 1.f;
        ProjectionMatrix[3][2] = -(far * near) / (far - near);
    }

    void Camera::SetViewDirection(Vec3 position, Vec3 direction, Vec3 up) {
        const Vec3 w{Wrath::Normalize(direction)};
        const Vec3 u{Wrath::Normalize(Wrath::Cross(w, up))};
        const Vec3 v{Wrath::Cross(w, u)};

        ViewMatrix = Mat4{1.0f};
        ViewMatrix[0][0] = u.x;
        ViewMatrix[1][0] = u.y;
        ViewMatrix[2][0] = u.z;
        ViewMatrix[0][1] = v.x;
        ViewMatrix[1][1] = v.y;
        ViewMatrix[2][1] = v.z;
        ViewMatrix[0][2] = w.x;
        ViewMatrix[1][2] = w.y;
        ViewMatrix[2][2] = w.z;
        ViewMatrix[3][0] = -Wrath::Dot(u, position);
        ViewMatrix[3][1] = -Wrath::Dot(v, position);
        ViewMatrix[3][2] = -Wrath::Dot(w, position);

        InverseViewMatrix = Mat4{1.0f};
        InverseViewMatrix[0][0] = u.x;
        InverseViewMatrix[0][1] = u.y;
        InverseViewMatrix[0][2] = u.z;
        InverseViewMatrix[1][0] = v.x;
        InverseViewMatrix[1][1] = v.y;
        InverseViewMatrix[1][2] = v.z;
        InverseViewMatrix[2][0] = w.x;
        InverseViewMatrix[2][1] = w.y;
        InverseViewMatrix[2][2] = w.z;
        InverseViewMatrix[3][0] = position.x;
        InverseViewMatrix[3][1] = position.y;
        InverseViewMatrix[3][2] = position.z;
    }

    void Camera::SetViewTarget(Vec3 position, Vec3 target, Vec3 up) {
        SetViewDirection(position, target - position, up);
    }

    void Camera::SetViewXYZ(Vec3 position, Vec3 rotation) {
        const float c3 = Wrath::Cos(rotation.z);
        const float s3 = Wrath::Sin(rotation.z);
        const float c2 = Wrath::Cos(rotation.x);
        const float s2 = Wrath::Sin(rotation.x);
        const float c1 = Wrath::Cos(rotation.y);
        const float s1 = Wrath::Sin(rotation.y);
        const Vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
        const Vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
        const Vec3 w{(c2 * s1), (-s2), (c1 * c2)};
        ViewMatrix = Mat4{1.f};
        ViewMatrix[0][0] = u.x;
        ViewMatrix[1][0] = u.y;
        ViewMatrix[2][0] = u.z;
        ViewMatrix[0][1] = v.x;
        ViewMatrix[1][1] = v.y;
        ViewMatrix[2][1] = v.z;
        ViewMatrix[0][2] = w.x;
        ViewMatrix[1][2] = w.y;
        ViewMatrix[2][2] = w.z;
        ViewMatrix[3][0] = -Wrath::Dot(u, position);
        ViewMatrix[3][1] = -Wrath::Dot(v, position);
        ViewMatrix[3][2] = -Wrath::Dot(w, position);

        InverseViewMatrix = Mat4{1.0f};
        InverseViewMatrix[0][0] = u.x;
        InverseViewMatrix[0][1] = u.y;
        InverseViewMatrix[0][2] = u.z;
        InverseViewMatrix[1][0] = v.x;
        InverseViewMatrix[1][1] = v.y;
        InverseViewMatrix[1][2] = v.z;
        InverseViewMatrix[2][0] = w.x;
        InverseViewMatrix[2][1] = w.y;
        InverseViewMatrix[2][2] = w.z;
        InverseViewMatrix[3][0] = position.x;
        InverseViewMatrix[3][1] = position.y;
        InverseViewMatrix[3][2] = position.z;
    }
} // namespace XIV