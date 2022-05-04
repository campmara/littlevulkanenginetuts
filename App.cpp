#include "App.h"

namespace XIV {
    void App::Run() {
        while (!window.ShouldClose()) {
            glfwPollEvents();
        }
    }
} // namespace XIV