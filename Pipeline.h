#ifndef PIPELINE_H
#define PIPELINE_H

#include "Core.h"
#include "Device.h"

#include <string>
#include <vector>

namespace XIV {
    struct PipelineConfigInfo {
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

        static PipelineConfigInfo DefaultConfigInfo(u32 width, u32 height);

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