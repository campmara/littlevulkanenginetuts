#ifndef APP_H
#define APP_H

#include "Window.h"
#include "Pipeline.h"

namespace XIV {
    class App {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void Run();

    private:
        Window window{WIDTH, HEIGHT, "AYO VULKAN!!!"};
        Pipeline pipeline{"shaders/simple.vert.spv", "shaders/simple.frag.spv"};
    };
} // namespace XIV

#endif