#include "systems/pointlightsystem.h"

#include "wrath.h"

#include <array>
#include <cassert>
#include <map>
#include <stdexcept>

namespace XIV::Systems {
    struct PointLightPushConstants {
        Vec4 Position{};
        Vec4 Color{};
        float Radius;
    };

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
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PointLightPushConstants);

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
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void PointLightSystem::CreatePipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultConfigInfo(pipelineConfig);
        Pipeline::EnableAlphaBlending(pipelineConfig);
        pipelineConfig.AttributeDescriptions.clear();
        pipelineConfig.BindingDescriptions.clear();
        pipelineConfig.RenderPass = renderPass;
        pipelineConfig.PipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device,
                                              "res/shaders/light_point.vert.spv",
                                              "res/shaders/light_point.frag.spv",
                                              pipelineConfig);
    }

    void PointLightSystem::Update(FrameInfo &frameInfo, GlobalUbo &ubo) {
        auto rotateLight =
            Wrath::Rotate(Mat4(1.0f), 0.5f * frameInfo.FrameTime, {0.0f, -1.0f, 0.0f});
        int lightIndex = 0;
        for (auto &kv : frameInfo.GameObjects) {
            auto &obj = kv.second;
            if (obj.PointLight == nullptr) {
                continue;
            }

            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

            // update light position
            obj.Transform.Translation = Vec3(rotateLight * Vec4(obj.Transform.Translation, 1.0f));

            // copy light to ubo
            ubo.PointLights[lightIndex].Position = Vec4(obj.Transform.Translation, 1.0f);
            ubo.PointLights[lightIndex].Color = Vec4(obj.Color, obj.PointLight->LightIntensity);

            lightIndex += 1;
        }
        ubo.NumLights = lightIndex;
    }

    void PointLightSystem::Render(FrameInfo &frameInfo) {
        // sort lights
        std::map<float, GameObject::id_t> sorted;
        for (auto &kv : frameInfo.GameObjects) {
            auto &obj = kv.second;
            if (obj.PointLight == nullptr)
                continue;

            // calculate distance
            auto offset = frameInfo.Camera.GetPosition() - obj.Transform.Translation;
            float disSquared = Wrath::Dot(offset, offset);
            sorted[disSquared] = obj.Id;
        }

        pipeline->Bind(frameInfo.CommandBuffer);

        vkCmdBindDescriptorSets(frameInfo.CommandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout,
                                0,
                                1,
                                &frameInfo.GlobalDescriptorSet,
                                0,
                                nullptr);

        // iterate through sorted lights in reverse order
        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
            // use game obj id to find light object
            auto &obj = frameInfo.GameObjects.at(it->second);

            PointLightPushConstants push{};
            push.Position = Vec4(obj.Transform.Translation, 1.0f);
            push.Color = Vec4(obj.Color, obj.PointLight->LightIntensity);
            push.Radius = obj.Transform.Scale.x;

            vkCmdPushConstants(frameInfo.CommandBuffer,
                               pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(PointLightPushConstants),
                               &push);
            vkCmdDraw(frameInfo.CommandBuffer, 6, 1, 0, 0);
        }
    }

} // namespace XIV::Systems