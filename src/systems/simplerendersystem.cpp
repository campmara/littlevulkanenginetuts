#include "systems/simplerendersystem.h"
#include "wrath.h"

#include <array>
#include <cassert>
#include <stdexcept>

namespace XIV::Systems {
    struct SimplePushConstantData {
        Mat4 ModelMatrix{1.0f};
        Mat4 NormalMatrix{1.0f};
    };

    SimpleRenderSystem::SimpleRenderSystem(Device &device,
                                           VkRenderPass renderPass,
                                           VkDescriptorSetLayout globalSetLayout)
        : device{device} {
        CreatePipelineLayout(globalSetLayout);
        CreatePipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem() {
        vkDestroyPipelineLayout(device.VulkanDevice, pipelineLayout, nullptr);
    }

    void SimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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
                                              "res/shaders/simple.vert.spv",
                                              "res/shaders/simple.frag.spv",
                                              pipelineConfig);
    }

    void SimpleRenderSystem::RenderGameObjects(FrameInfo &frameInfo) {
        pipeline->Bind(frameInfo.CommandBuffer);

        vkCmdBindDescriptorSets(frameInfo.CommandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout,
                                0,
                                1,
                                &frameInfo.GlobalDescriptorSet,
                                0,
                                nullptr);

        for (auto &kv : frameInfo.GameObjects) {
            // Get the object from the map and check for the model.
            auto &obj = kv.second;
            if (obj.Model == nullptr) {
                continue;
            }

            SimplePushConstantData push{};
            push.ModelMatrix = obj.Transform.Matrix4();
            push.NormalMatrix = obj.Transform.NormalMatrix();
            vkCmdPushConstants(frameInfo.CommandBuffer,
                               pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(SimplePushConstantData),
                               &push);
            obj.Model->Bind(frameInfo.CommandBuffer);
            obj.Model->Draw(frameInfo.CommandBuffer);
        }
    }
} // namespace XIV::Systems