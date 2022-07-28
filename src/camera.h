#ifndef CAMERA_H
#define CAMERA_H

#include "wrath.h"

namespace XIV {
    class Camera {
    public:
        void SetOrthographicProjection(
            float left, float right, float top, float bottom, float near, float far);
        void SetPerspectiveProjection(float fovY, float aspect, float near, float far);

        const Vec3 GetPosition() const {
            return Vec3(InverseViewMatrix[3]);
        }

        void SetViewDirection(Vec3 position, Vec3 direction, Vec3 up = Vec3{0.0f, -1.0f, 0.0f});
        void SetViewTarget(Vec3 position, Vec3 target, Vec3 up = Vec3{0.0f, -1.0f, 0.0f});
        void SetViewXYZ(Vec3 position, Vec3 rotation);

        Mat4 ProjectionMatrix{1.0f};
        Mat4 ViewMatrix{1.0f};
        Mat4 InverseViewMatrix{1.0f};
    };
} // namespace XIV

#endif