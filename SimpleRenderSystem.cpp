#include "SimpleRenderSystem.h"
#include "Math.h"

#include <array>
#include <cassert>
#include <stdexcept>

namespace XIV {
    struct SimplePushConstantData {
        Mat4 Transform{1.0f};
        Mat4 NormalMatrix{1.0f};
    };

    SimpleRenderSystem::SimpleRenderSystem(Device &device, VkRenderPass renderPass)
        : device{device} {
        CreatePipelineLayout();
        CreatePipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem() {
        vkDestroyPipelineLayout(device.VulkanDevice, pipelineLayout, nullptr);
    }

    void SimpleRenderSystem::CreatePipelineLayout() {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.VulkanDevice,
                                   &pipelineLayoutInfo,
                                   nullptr,
                                   &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultConfigInfo(pipelineConfig);
        pipelineConfig.RenderPass = renderPass;
        pipelineConfig.PipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device,
                                              "shaders/simple.vert.spv",
                                              "shaders/simple.frag.spv",
                                              pipelineConfig);
    }

    void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer,
                                               std::vector<GameObject> &gameObjects,
                                               const Camera &camera) {
        pipeline->Bind(commandBuffer);

        auto projectionView = camera.ProjectionMatrix * camera.ViewMatrix;

        for (auto &obj : gameObjects) {
            SimplePushConstantData push{};
            auto modelMatrix = obj.Transform.Matrix4();
            push.Transform = projectionView * modelMatrix;
            push.NormalMatrix = obj.Transform.NormalMatrix();
            vkCmdPushConstants(commandBuffer,
                               pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(SimplePushConstantData),
                               &push);
            obj.Model->Bind(commandBuffer);
            obj.Model->Draw(commandBuffer);
        }
    }
} // namespace XIV