#ifndef SIMPLE_RENDER_SYSTEM_H
#define SIMPLE_RENDER_SYSTEM_H

#include "Camera.h"
#include "Device.h"
#include "FrameInfo.h"
#include "GameObject.h"
#include "Pipeline.h"

#include <memory>
#include <vector>

namespace XIV {
    class SimpleRenderSystem {
    public:
        SimpleRenderSystem(Device &device,
                           VkRenderPass renderPass,
                           VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();
        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void RenderGameObjects(FrameInfo &frameInfo);

    private:
        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void CreatePipeline(VkRenderPass renderPass);

        Device &device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
} // namespace XIV

#endif
