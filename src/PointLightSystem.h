#ifndef POINT_LIGHT_SYSTEM
#define POINT_LIGHT_SYSTEM

#include "Camera.h"
#include "Device.h"
#include "FrameInfo.h"
#include "GameObject.h"
#include "Pipeline.h"

#include <memory>
#include <vector>

namespace XIV {
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
} // namespace XIV

#endif