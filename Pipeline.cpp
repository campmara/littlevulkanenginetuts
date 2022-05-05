#include "Pipeline.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace XIV {
    Pipeline::Pipeline(Device &device,
                       const std::string &vertPath,
                       const std::string &fragPath,
                       const PipelineConfigInfo &configInfo)
        : device{device} {
        CreateGraphicsPipeline(vertPath, fragPath, configInfo);
    }

    Pipeline::~Pipeline() {
        vkDestroyShaderModule(device.VulkanDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(device.VulkanDevice, vertShaderModule, nullptr);
        vkDestroyPipeline(device.VulkanDevice, graphicsPipeline, nullptr);
    }

    void Pipeline::DefaultConfigInfo(PipelineConfigInfo &configInfo, u32 width, u32 height) {
        configInfo.InputAssemblyInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.Viewport.x = 0.0f;
        configInfo.Viewport.y = 0.0f;
        configInfo.Viewport.width = static_cast<float>(width);
        configInfo.Viewport.height = static_cast<float>(height);
        configInfo.Viewport.minDepth = 0.0f;
        configInfo.Viewport.maxDepth = 1.0f;

        configInfo.Scissor.offset = {0, 0};
        configInfo.Scissor.extent = {width, height};

        configInfo.RasterizationInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.RasterizationInfo.lineWidth = 1.0f;
        configInfo.RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
        configInfo.RasterizationInfo.depthBiasClamp = 0.0f;          // Optional
        configInfo.RasterizationInfo.depthBiasSlopeFactor = 0.0f;    // Optional

        configInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.MultisampleInfo.minSampleShading = 1.0f;          // Optional
        configInfo.MultisampleInfo.pSampleMask = nullptr;            // Optional
        configInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
        configInfo.MultisampleInfo.alphaToOneEnable = VK_FALSE;      // Optional

        configInfo.ColorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        configInfo.ColorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        configInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        configInfo.ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
        configInfo.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        configInfo.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        configInfo.ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional

        configInfo.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.ColorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
        configInfo.ColorBlendInfo.attachmentCount = 1;
        configInfo.ColorBlendInfo.pAttachments = &configInfo.ColorBlendAttachment;
        configInfo.ColorBlendInfo.blendConstants[0] = 0.0f; // Optional
        configInfo.ColorBlendInfo.blendConstants[1] = 0.0f; // Optional
        configInfo.ColorBlendInfo.blendConstants[2] = 0.0f; // Optional
        configInfo.ColorBlendInfo.blendConstants[3] = 0.0f; // Optional

        configInfo.DepthStencilInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.DepthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.DepthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.DepthStencilInfo.minDepthBounds = 0.0f; // Optional
        configInfo.DepthStencilInfo.maxDepthBounds = 1.0f; // Optional
        configInfo.DepthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.DepthStencilInfo.front = {}; // Optional
        configInfo.DepthStencilInfo.back = {};  // Optional
    }

    void Pipeline::Bind(VkCommandBuffer commandBuffer) {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    }

    std::vector<char> Pipeline::ReadFile(const std::string &fileName) {
        std::ifstream file{fileName, std::ios::ate | std::ios::binary};

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + fileName);
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    void Pipeline::CreateGraphicsPipeline(const std::string &vertPath,
                                          const std::string &fragPath,
                                          const PipelineConfigInfo &configInfo) {
        assert(configInfo.PipelineLayout != nullptr &&
               "Cannot create graphics pipeline: no PipelineLayout provided in config info.");
        assert(configInfo.RenderPass != nullptr &&
               "Cannot create graphics pipeline: no RenderPass provided in config info.");

        std::vector<char> vertCode = ReadFile(vertPath);
        std::vector<char> fragCode = ReadFile(fragPath);

        CreateShaderModule(vertCode, &vertShaderModule);
        CreateShaderModule(fragCode, &fragShaderModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &configInfo.Viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &configInfo.Scissor;

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.InputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &configInfo.RasterizationInfo;
        pipelineInfo.pMultisampleState = &configInfo.MultisampleInfo;
        pipelineInfo.pColorBlendState = &configInfo.ColorBlendInfo;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.pDepthStencilState = &configInfo.DepthStencilInfo;

        pipelineInfo.layout = configInfo.PipelineLayout;
        pipelineInfo.renderPass = configInfo.RenderPass;
        pipelineInfo.subpass = configInfo.Subpass;

        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1;              // Optional

        if (vkCreateGraphicsPipelines(device.VulkanDevice,
                                      VK_NULL_HANDLE,
                                      1,
                                      &pipelineInfo,
                                      nullptr,
                                      &graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create graphics pipeline.");
        }

        vkDestroyShaderModule(device.VulkanDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(device.VulkanDevice, vertShaderModule, nullptr);
        fragShaderModule = VK_NULL_HANDLE;
        vertShaderModule = VK_NULL_HANDLE;
    }

    void Pipeline::CreateShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const u32 *>(code.data());

        if (vkCreateShaderModule(device.VulkanDevice, &createInfo, nullptr, shaderModule) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module.");
        }
    }
} // namespace XIV