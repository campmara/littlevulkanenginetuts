#ifndef POINT_LIGHT_SYSTEM
#define POINT_LIGHT_SYSTEM

#include "Render/FrameInfo.h"
#include "Render/Pipeline.h"
#include "Render/Device.h"
#include "Camera.h"
#include "GameObject.h"

#include <memory>
#include <vector>

using namespace XIV::Render;

namespace XIV::Systems {
    class PointLightSystem {
    public:
        PointLightSystem(Device &device,
                         VkRenderPass renderPass,
                         VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();
        PointLightSystem(const PointLightSystem &) = delete;
        PointLightSystem &operator=(const PointLightSystem &) = delete;

        void Render(FrameInfo &frameInfo);

    private:
        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void CreatePipeline(VkRenderPass renderPass);

        Device &device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
} // namespace XIV::Systems

#endif