#include "App.h"

#include <array>
#include <stdexcept>

namespace XIV {
    App::App() {
        LoadModels();
        CreatePipelineLayout();
        RecreateSwapChain();
        CreateCommandBuffers();
    }

    App::~App() {
        vkDestroyPipelineLayout(device.VulkanDevice, pipelineLayout, nullptr);
    }

    void App::Run() {
        while (!window.ShouldClose()) {
            glfwPollEvents();
            DrawFrame();
        }

        vkDeviceWaitIdle(device.VulkanDevice);
    }

    void App::LoadModels() {
        std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        model = std::make_unique<Model>(device, vertices);
    }

    void App::CreatePipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(device.VulkanDevice,
                                   &pipelineLayoutInfo,
                                   nullptr,
                                   &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout.");
        }
    }

    void App::CreatePipeline() {
        assert(swapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultConfigInfo(pipelineConfig);
        pipelineConfig.RenderPass = swapChain->RenderPass;
        pipelineConfig.PipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device,
                                              "shaders/simple.vert.spv",
                                              "shaders/simple.frag.spv",
                                              pipelineConfig);
    }

    void App::CreateCommandBuffers() {
        commandBuffers.resize(swapChain->GetImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.CommandPool;
        allocInfo.commandBufferCount = static_cast<u32>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.VulkanDevice, &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers.");
        }
    }

    void App::FreeCommandBuffers() {
        vkFreeCommandBuffers(device.VulkanDevice,
                             device.CommandPool,
                             static_cast<u32>(commandBuffers.size()),
                             commandBuffers.data());
        commandBuffers.clear();
    }

    void App::DrawFrame() {
        u32 imageIndex;
        auto result = swapChain->AcquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image.");
        }

        RecordCommandBuffer(imageIndex);
        result = swapChain->SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            window.WasFrameBufferResized) {
            window.WasFrameBufferResized = false;
            RecreateSwapChain();
            return;
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image.");
        }
    }

    void App::RecreateSwapChain() {
        auto extent = window.GetBoundsAsExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = window.GetBoundsAsExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.VulkanDevice);

        if (swapChain == nullptr) {
            swapChain = std::make_unique<SwapChain>(device, extent);
        } else {
            swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));
            if (swapChain->GetImageCount() != commandBuffers.size()) {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }

        CreatePipeline();
    }

    void App::RecordCommandBuffer(int imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->RenderPass;
        renderPassInfo.framebuffer = swapChain->Framebuffers[imageIndex];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->Extent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<u32>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex],
                             &renderPassInfo,
                             VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->Extent.width);
        viewport.height = static_cast<float>(swapChain->Extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, swapChain->Extent};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        pipeline->Bind(commandBuffers[imageIndex]);
        model->Bind(commandBuffers[imageIndex]);
        model->Draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
} // namespace XIV