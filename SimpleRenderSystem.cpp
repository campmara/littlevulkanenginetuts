#include "SimpleRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

namespace XIV {
    struct SimplePushConstantData {
        glm::mat4 transform{1.0f};
        alignas(16) glm::vec3 color;
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

        for (auto &obj : gameObjects) {
            obj.Transform.Rotation.y =
                glm::mod(obj.Transform.Rotation.y + 0.01f, glm::two_pi<float>());
            obj.Transform.Rotation.x =
                glm::mod(obj.Transform.Rotation.x + 0.005f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.color = obj.Color;
            push.transform = camera.ProjectionMatrix * obj.Transform.Mat4();

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