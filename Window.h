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

        int Width;
        int Height;
        bool WasFrameBufferResized = false;

        bool ShouldClose() {
            return glfwWindowShouldClose(window);
        }

        VkExtent2D GetBoundsAsExtent() {
            return {static_cast<uint32_t>(Width), static_cast<uint32_t>(Height)};
        }

        void CreateSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void OnFrameBufferResized(GLFWwindow *window, int width, int height);
        void InitWindow();

        const char *name;
        GLFWwindow *window;
    };
} // namespace XIV

#endif