#include "App.h"

#include <array>
#include <stdexcept>

namespace XIV {
    App::App() {
        LoadModels();
        CreatePipelineLayout();
        CreatePipeline();
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
        // just a triangle
        // std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};

        // sierpinski triangle
        std::vector<Model::Vertex> vertices{};
        Sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});

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
        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultConfigInfo(pipelineConfig, swapChain.Width(), swapChain.Height());
        pipelineConfig.RenderPass = swapChain.RenderPass;
        pipelineConfig.PipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device,
                                              "shaders/simple.vert.spv",
                                              "shaders/simple.frag.spv",
                                              pipelineConfig);
    }

    void App::CreateCommandBuffers() {
        commandBuffers.resize(swapChain.GetImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.CommandPool;
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.VulkanDevice, &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers.");
        }

        for (size_t i = 0; i < commandBuffers.size(); ++i) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Failed to begin recording command buffer.");
            }

            // Set up clear color values
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChain.RenderPass;
            renderPassInfo.framebuffer = swapChain.SwapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChain.SwapChainExtent;
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            pipeline->Bind(commandBuffers[i]);
            model->Bind(commandBuffers[i]);
            model->Draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to record command buffer.");
            }
        }
    }

    void App::DrawFrame() {
        uint32_t imageIndex;
        auto result = swapChain.AcquireNextImage(&imageIndex);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        result = swapChain.SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

    void App::Sierpinski(std::vector<Model::Vertex> &vertices,
                         int depth,
                         glm::vec2 left,
                         glm::vec2 right,
                         glm::vec2 top) {
        if (depth <= 0) {
            vertices.push_back({top});
            vertices.push_back({right});
            vertices.push_back({left});
        } else {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            Sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            Sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            Sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }
} // namespace XIV