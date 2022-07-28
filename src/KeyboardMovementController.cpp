#include "KeyboardMovementController.h"

#include "Wrath.h"

namespace XIV {
    void KeyboardMovementController::MoveInPlaneXZ(GLFWwindow *window,
                                                   float dt,
                                                   GameObject &gameObject) {
        Vec3 rotate{0};
        if (glfwGetKey(window, Keys.LookRight) == GLFW_PRESS) {
            rotate.y += 1.0f;
        }
        if (glfwGetKey(window, Keys.LookLeft) == GLFW_PRESS) {
            rotate.y -= 1.0f;
        }
        if (glfwGetKey(window, Keys.LookUp) == GLFW_PRESS) {
            rotate.x += 1.0f;
        }
        if (glfwGetKey(window, Keys.LookDown) == GLFW_PRESS) {
            rotate.x -= 1.0f;
        }

        if (Wrath::Dot(rotate, rotate) > Wrath::Epsilon()) {
            gameObject.Transform.Rotation += LookSpeed * dt * Wrath::Normalize(rotate);
        }

        // limit pitch values between about +/- 85ish degrees
        gameObject.Transform.Rotation.x =
            Wrath::Clamp(gameObject.Transform.Rotation.x, -1.5f, 1.5f);
        gameObject.Transform.Rotation.y =
            Wrath::Mod(gameObject.Transform.Rotation.y, Wrath::TwoPi());

        float yaw = gameObject.Transform.Rotation.y;
        const Vec3 forwardDir{sin(yaw), 0.0f, cos(yaw)};
        const Vec3 rightDir{forwardDir.z, 0.0f, -forwardDir.x};
        const Vec3 upDir{0.0f, -1.0f, 0.0f};

        Vec3 moveDir{0.f};
        if (glfwGetKey(window, Keys.MoveForward) == GLFW_PRESS) {
            moveDir += forwardDir;
        }
        if (glfwGetKey(window, Keys.MoveBackward) == GLFW_PRESS) {
            moveDir -= forwardDir;
        }
        if (glfwGetKey(window, Keys.MoveRight) == GLFW_PRESS) {
            moveDir += rightDir;
        }
        if (glfwGetKey(window, Keys.MoveLeft) == GLFW_PRESS) {
            moveDir -= rightDir;
        }
        if (glfwGetKey(window, Keys.MoveUp) == GLFW_PRESS) {
            moveDir += upDir;
        }
        if (glfwGetKey(window, Keys.MoveDown) == GLFW_PRESS) {
            moveDir -= upDir;
        }

        if (Wrath::Dot(moveDir, moveDir) > Wrath::Epsilon()) {
            gameObject.Transform.Translation += MoveSpeed * dt * Wrath::Normalize(moveDir);
        }
    }
} // namespace XIV