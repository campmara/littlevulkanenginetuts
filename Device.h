#ifndef DEVICE_H
#define DEVICE_H

#include "Core.h"
#include "Window.h"

#include <string>
#include <vector>

namespace XIV {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    struct QueueFamilyIndices {
        u32 GraphicsFamily;
        u32 PresentFamily;
        bool GraphicsFamilyHasValue = false;
        bool PresentFamilyHasValue = false;

        bool IsComplete() {
            return GraphicsFamilyHasValue && PresentFamilyHasValue;
        }
    };

    class Device {
    public:
#ifdef DEBUG
        const bool VALIDATION_LAYERS_ENABLED = true;
#else
        const bool VALIDATION_LAYERS_ENABLED = false;
#endif

        Device(Window &window);
        ~Device();

        Device(const Device &) = delete;
        void operator=(const Device &) = delete;
        Device(Device &&) = delete;
        Device &operator=(Device &&) = delete;

        SwapChainSupportDetails GetSwapChainSupport() {
            return QuerySwapChainSupport(physicalDevice);
        }

        QueueFamilyIndices FindPhysicalQueueFamilies() {
            return FindQueueFamilies(physicalDevice);
        }

        u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
        VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates,
                                     VkImageTiling tiling,
                                     VkFormatFeatureFlags features);

        // Buffer-related helpers
        void CreateBuffer(VkDeviceSize size,
                          VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties,
                          VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void
        CopyBufferToImage(VkBuffer buffer, VkImage image, u32 width, u32 height, u32 layerCount);
        void CreateImageWithInfo(const VkImageCreateInfo &imageInfo,
                                 VkMemoryPropertyFlags properties,
                                 VkImage &image,
                                 VkDeviceMemory &imageMemory);

        VkCommandPool CommandPool;
        VkDevice VulkanDevice;
        VkSurfaceKHR Surface;
        VkQueue GraphicsQueue;
        VkQueue PresentQueue;
        VkPhysicalDeviceProperties Properties;

    private:
        const std::vector<const char *> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // Vulkan-specific
        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateCommandPool();

        // Helpers
        bool IsDeviceSuitable(VkPhysicalDevice device);
        std::vector<const char *> GetRequiredExtensions();
        bool CheckValidationLayerSupport();
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void HasGflwRequiredInstanceExtensions();
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        Window &window;
    };
} // namespace XIV

#endif