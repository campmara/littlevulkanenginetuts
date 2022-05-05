#include "Window.h"

#include <stdexcept>

namespace XIV {
    Window::Window(int w, int h, const char *name) : Width(w), Height(h), name(name) {
        InitWindow();
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::CreateSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface.");
        }
    }

    void Window::OnFrameBufferResized(GLFWwindow *window, int width, int height) {
        auto windowPtr = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        windowPtr->WasFrameBufferResized = true;
        windowPtr->Width = width;
        windowPtr->Height = height;
    }

    void Window::InitWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(Width, Height, name, nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, OnFrameBufferResized);
    }
} // namespace XIV