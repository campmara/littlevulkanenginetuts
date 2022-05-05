#ifndef APP_H
#define APP_H

#include "Device.h"
#include "Model.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "Window.h"

#include <memory>
#include <vector>

namespace XIV {
    class App {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();
        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void Run();

    private:
        void LoadModels();
        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffers();
        void DrawFrame();

        // just for fun
        void Sierpinski(std::vector<Model::Vertex> &vertices,
                        int depth,
                        glm::vec2 left,
                        glm::vec2 right,
                        glm::vec2 top);

        Window window{WIDTH, HEIGHT, "AYO VULKAN!!!"};
        Device device{window};
        SwapChain swapChain{device, window.GetBoundsAsExtent()};

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;

        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<Model> model;
    };
} // namespace XIV

#endif