#include "window.h"

#include <stdexcept>

namespace XIV::Render {
    Window::Window(int w, int h, const char *name) : Width(w), Height(h), name(name) {
        InitWindow();
    }

    Window::~Window() {
        glfwDestroyWindow(GlfwWindow);
        glfwTerminate();
    }

    void Window::CreateSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, GlfwWindow, nullptr, surface) != VK_SUCCESS) {
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

        GlfwWindow = glfwCreateWindow(Width, Height, name, nullptr, nullptr);
        glfwSetWindowUserPointer(GlfwWindow, this);
        glfwSetFramebufferSizeCallback(GlfwWindow, OnFrameBufferResized);
    }
} // namespace XIV