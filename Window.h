#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace XIV {
    class Window {
    public:
        Window(int w, int h, const char *name);
        ~Window();
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        const int width;
        const int height;

        bool ShouldClose() {
            return glfwWindowShouldClose(window);
        }

        void CreateSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        void InitWindow();

        const char *name;
        GLFWwindow *window;
    };
} // namespace XIV

#endif