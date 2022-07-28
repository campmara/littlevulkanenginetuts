#ifndef PIPELINE_H
#define PIPELINE_H

#include "core.h"
#include "device.h"

#include <string>
#include <vector>

namespace XIV::Render {
    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo &) = delete;
        PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

        std::vector<VkVertexInputBindingDescription> BindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> AttributeDescriptions{};
        VkPipelineViewportStateCreateInfo ViewportInfo;
        VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo RasterizationInfo;
        VkPipelineMultisampleStateCreateInfo MultisampleInfo;
        VkPipelineColorBlendAttachmentState ColorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
        std::vector<VkDynamicState> DynamicStateEnables;
        VkPipelineDynamicStateCreateInfo DynamicStateInfo;
        VkPipelineLayout PipelineLayout = nullptr;
        VkRenderPass RenderPass = nullptr;
        uint32_t Subpass = 0;
    };

    class Pipeline {
    public:
        Pipeline(Device &device,
                 const std::string &vertPath,
                 const std::string &fragPath,
                 const PipelineConfigInfo &configInfo);
        ~Pipeline();
        Pipeline(const Pipeline &) = delete;
        Pipeline &operator=(const Pipeline &) = delete;

        static void DefaultConfigInfo(PipelineConfigInfo &configInfo);

        void Bind(VkCommandBuffer commandBuffer);

    private:
        std::vector<char> ReadFile(const std::string &filePath);

        void CreateGraphicsPipeline(const std::string &vertPath,
                                    const std::string &fragPath,
                                    const PipelineConfigInfo &configInfo);
        void CreateShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        Device &device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
} // namespace XIV::Render

#endif