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

        void SetViewDirection(glm::vec3 position,
                              glm::vec3 direction,
                              glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
        void SetViewTarget(glm::vec3 position,
                           glm::vec3 target,
                           glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
        void SetViewXYZ(glm::vec3 position, glm::vec3 rotation);

        glm::mat4 ProjectionMatrix{1.0f};
        glm::mat4 ViewMatrix{1.0f};
    };
} // namespace XIV

#endif