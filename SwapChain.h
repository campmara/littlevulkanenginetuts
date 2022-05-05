#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

#include "Core.h"
#include "Device.h"

#include <vulkan/vulkan.h>

#include <memory>
#include <string>
#include <vector>

namespace XIV {
    class SwapChain {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        SwapChain(Device &device, VkExtent2D windowExtent);
        SwapChain(Device &device, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
        ~SwapChain();
        SwapChain(const SwapChain &) = delete;
        SwapChain &operator=(const SwapChain &) = delete;

        size_t GetImageCount() {
            return images.size();
        }

        u32 Width() {
            return Extent.width;
        }

        u32 Height() {
            return Extent.height;
        }

        float GetExtentAspectRatio() {
            return static_cast<float>(Extent.width) / static_cast<float>(Extent.height);
        }

        VkFormat FindDepthFormat();
        VkResult AcquireNextImage(uint32_t *imageIndex);
        VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

        VkFormat ImageFormat;
        VkExtent2D Extent;

        std::vector<VkFramebuffer> Framebuffers;
        std::vector<VkImageView> ImageViews;

        VkRenderPass RenderPass;

    private:
        // Vulkan-specific
        void Init();
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
        std::shared_ptr<SwapChain> oldSwapChain;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemories;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> images;

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