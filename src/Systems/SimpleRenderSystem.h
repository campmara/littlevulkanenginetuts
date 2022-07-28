#ifndef SIMPLE_RENDER_SYSTEM_H
#define SIMPLE_RENDER_SYSTEM_H

#include "render/device.h"
#include "render/frameinfo.h"
#include "render/pipeline.h"
#include "gameobject.h"
#include "camera.h"

#include <memory>
#include <vector>

using namespace XIV::Render;

namespace XIV::Systems {
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
} // namespace XIV::Systems

#endif
