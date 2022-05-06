#ifndef RENDERER_H
#define RENDERER_H

#include "Device.h"
#include "SwapChain.h"
#include "Window.h"

#include <cassert>
#include <memory>
#include <vector>

namespace XIV {
    class Renderer {
    public:
        Renderer(Window &window, Device &device);
        ~Renderer();
        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        VkRenderPass GetSwapChainRenderPass() const {
            return swapChain->RenderPass;
        }

        VkCommandBuffer GetCurrentCommandBuffer() const {
            assert(IsFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int GetFrameIndex() const {
            assert(IsFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer BeginFrame();
        void EndFrame();
        void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

        bool IsFrameStarted{false};

    private:
        void CreateCommandBuffers();
        void FreeCommandBuffers();
        void RecreateSwapChain();

        Window &window;
        Device &device;
        std::unique_ptr<SwapChain> swapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        u32 currentImageIndex;
        int currentFrameIndex{0};
    };
} // namespace XIV

#endif