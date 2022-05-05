#ifndef PIPELINE_H
#define PIPELINE_H

#include "Core.h"
#include "Device.h"

#include <string>
#include <vector>

namespace XIV {
    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo &) = delete;
        PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

        VkViewport Viewport;
        VkRect2D Scissor;
        VkPipelineViewportStateCreateInfo ViewportInfo;
        VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo RasterizationInfo;
        VkPipelineMultisampleStateCreateInfo MultisampleInfo;
        VkPipelineColorBlendAttachmentState ColorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
        VkPipelineLayout PipelineLayout = nullptr;
        VkRenderPass RenderPass = nullptr;
        u32 Subpass = 0;
    };

    class Pipeline {
    public:
        Pipeline(Device &device,
                 const std::string &vertPath,
                 const std::string &fragPath,
                 const PipelineConfigInfo &configInfo);
        ~Pipeline();
        Pipeline(const Pipeline &) = delete;
        void operator=(const Pipeline &) = delete;

        static void DefaultConfigInfo(PipelineConfigInfo &configInfo, u32 width, u32 height);

        void Bind(VkCommandBuffer commandBuffer);

    private:
        std::vector<char> ReadFile(const std::string &fileName);

        void CreateGraphicsPipeline(const std::string &vertPath,
                                    const std::string &fragPath,
                                    const PipelineConfigInfo &configInfo);
        void CreateShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        Device &device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
} // namespace XIV

#endif