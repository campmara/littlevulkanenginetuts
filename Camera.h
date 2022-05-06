#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace XIV {
    class Camera {
    public:
        void SetOrthographicProjection(
            float left, float right, float top, float bottom, float near, float far);
        void SetPerspectiveProjection(float fovY, float aspect, float near, float far);

        glm::mat4 ProjectionMatrix{1.0f};
    };
} // namespace XIV

#endif