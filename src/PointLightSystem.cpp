#include "PointLightSystem.h"

#include "Math.h"

#include <array>
#include <cassert>
#include <stdexcept>

namespace XIV {
    PointLightSystem::PointLightSystem(Device &device,
                                       VkRenderPass renderPass,
                                       VkDescriptorSetLayout globalSetLayout)
        : device{device} {
        CreatePipelineLayout(globalSetLayout);
        CreatePipeline(renderPass);
    }

    PointLightSystem::~PointLightSystem() {
        vkDestroyPipelineLayout(device.VulkanDevice, pipelineLayout, nullptr);
    }

    void PointLightSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        // VkPushConstantRange pushConstantRange{};
        // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        // pushConstantRange.offset = 0;
        // pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(device.VulkanDevice,
                                   &pipelineLayoutInfo,
                                   nullptr,
                                   &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void PointLightSystem::CreatePipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultConfigInfo(pipelineConfig);
        pipelineConfig.AttributeDescriptions.clear();
        pipelineConfig.BindingDescriptions.clear();
        pipelineConfig.RenderPass = renderPass;
        pipelineConfig.PipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device,
                                              "shaders/light_point.vert.spv",
                                              "shaders/light_point.frag.spv",
                                              pipelineConfig);
    }

    void PointLightSystem::Render(FrameInfo &frameInfo) {
        pipeline->Bind(frameInfo.CommandBuffer);

        vkCmdBindDescriptorSets(frameInfo.CommandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout,
                                0,
                                1,
                                &frameInfo.GlobalDescriptorSet,
                                0,
                                nullptr);

        vkCmdDraw(frameInfo.CommandBuffer, 6, 1, 0, 0);
    }

} // namespace XIV