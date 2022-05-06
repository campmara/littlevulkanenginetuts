#ifndef SIMPLE_RENDER_SYSTEM_H
#define SIMPLE_RENDER_SYSTEM_H

#include "Camera.h"
#include "Device.h"
#include "GameObject.h"
#include "Pipeline.h"

#include <memory>
#include <vector>

namespace XIV {
    class SimpleRenderSystem {
    public:
        SimpleRenderSystem(Device &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();
        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void RenderGameObjects(VkCommandBuffer commandBuffer,
                               std::vector<GameObject> &gameObjects,
                               const Camera &camera);

    private:
        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);

        Device &device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
} // namespace XIV

#endif
