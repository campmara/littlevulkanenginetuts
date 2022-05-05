#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

#include "Core.h"
#include "Device.h"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace XIV {
    class SwapChain {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        SwapChain(Device &device, VkExtent2D windowExtent);
        ~SwapChain();
        SwapChain(const SwapChain &) = delete;
        void operator=(const SwapChain &) = delete;

        size_t GetImageCount() {
            return swapChainImages.size();
        }

        u32 Width() {
            return SwapChainExtent.width;
        }

        u32 Height() {
            return SwapChainExtent.height;
        }

        float GetExtentAspectRatio() {
            return static_cast<float>(SwapChainExtent.width) /
                   static_cast<float>(SwapChainExtent.height);
        }

        VkFormat FindDepthFormat();
        VkResult AcquireNextImage(uint32_t *imageIndex);
        VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

        VkFormat SwapChainImageFormat;
        VkExtent2D SwapChainExtent;

        std::vector<VkFramebuffer> SwapChainFramebuffers;
        std::vector<VkImageView> SwapChainImageViews;

        VkRenderPass RenderPass;

    private:
        // Vulkan-specific
        void CreateSwapChain();
        void CreateImageViews();
        void CreateDepthResources();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CreateSyncObjects();

        // Helpers
        VkSurfaceFormatKHR
        ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR
        ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        VkSwapchainKHR swapChain;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemories;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;

        Device &device;
        VkExtent2D windowExtent;

        size_t currentFrame = 0;
    };
} // namespace XIV

#endif