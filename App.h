#ifndef APP_H
#define APP_H

#include "Pipeline.h"
#include "Window.h"

namespace XIV {
    class App {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void Run();

    private:
        Window window{WIDTH, HEIGHT, "AYO VULKAN!!!"};
        Device device{window};
        Pipeline pipeline{device,
                          "shaders/simple.vert.spv",
                          "shaders/simple.frag.spv",
                          Pipeline::DefaultConfigInfo(WIDTH, HEIGHT)};
        VkPipelineLayout pipelineLayout;
    };
} // namespace XIV

#endif