#include "rendering/vulkan/VKPipeline.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
//TODO: Create Legio Asserts
#include <cassert>

namespace LG 
{
    VKPipeline::VKPipeline(
        VKDevice& device,
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& info
    )
        : m_device{ device }
    {
        createGrpahicsPipeline(vertFilepath, fragFilepath, info);
    }

    VKPipeline::~VKPipeline()
    {
        LG_CORE_INFO("\t VKPipeline deleted");
        vkDestroyShaderModule(m_device.GetDevice(), m_vertShaderModuel, nullptr);
        vkDestroyShaderModule(m_device.GetDevice(), m_fragShaderModuel, nullptr);
        vkDestroyPipeline(m_device.GetDevice(), m_graphicsPipeline, nullptr);
    }

    std::vector<char> VKPipeline::ReadFile(const std::string& filePath)
    {
        std::ifstream file{ filePath, std::ios::ate | std::ios::binary };

        if (!file.is_open()) 
        {
            throw std::runtime_error("File to open file: " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());

        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void VKPipeline::createGrpahicsPipeline(
        const std::string& vertFilepath, 
        const std::string& fragFilepath, 
        const PipelineConfigInfo& info
    )
    {
        assert(info.pipelineLayout != VK_NULL_HANDLE && "Cannot Create Graphics Pipeline: no pipelineLayout Provided in configInfo");
        assert(info.renderPass != VK_NULL_HANDLE && "Cannot Create Graphics Pipeline: no renderPass Provided in configInfo");

        auto vertCode = ReadFile(vertFilepath);
        auto fragCode = ReadFile(fragFilepath);

        createShaderModule(vertCode, &m_vertShaderModuel);
        createShaderModule(fragCode, &m_fragShaderModuel);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = m_vertShaderModuel;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = m_fragShaderModuel;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;

        VkPipelineViewportStateCreateInfo viewportInfo{};
        //Combine Viewport and Scissors
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &info.viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &info.scissors;

        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo.logicOpEnable = VK_FALSE;
        colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendInfo.attachmentCount = 1;
        colorBlendInfo.pAttachments = &info.colorBlendAttachment;
        colorBlendInfo.blendConstants[0] = 0.0f;
        colorBlendInfo.blendConstants[1] = 0.0f;
        colorBlendInfo.blendConstants[2] = 0.0f;
        colorBlendInfo.blendConstants[3] = 0.0f;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &info.inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &info.rasterizationInfo;
        pipelineInfo.pMultisampleState = &info.multisampleInfo;
        pipelineInfo.pColorBlendState = &colorBlendInfo;
        pipelineInfo.pDepthStencilState = &info.depthStencilInfo;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = info.pipelineLayout;
        pipelineInfo.renderPass = info.renderPass;
        pipelineInfo.subpass = info.subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_device.GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create Graphics Pipeline");
        }
    }
    void VKPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(m_device.GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create Shader Module");
        }
    }

    PipelineConfigInfo VKPipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};
        //1 - Input Assembly
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.viewport.x = 0.0f;
        configInfo.viewport.y = 0.0f;
        configInfo.viewport.width = static_cast<float>(width);
        configInfo.viewport.height = static_cast<float>(height);

        //Range for the viewport
        configInfo.viewport.minDepth = 0.0f;
        configInfo.viewport.maxDepth = 1.0f;
        configInfo.scissors.offset = { 0,0 };
        configInfo.scissors.extent = { width,height };

        //2 - Rasterization
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; //Forces Z component to be between 0 and 1
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL; //Drawing Triangles
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE; //What side of the triangle we are looking at
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE; //alter depth values
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

        //Rasterization Edge Handling
        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;
        configInfo.multisampleInfo.pSampleMask = nullptr;
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

        //3 - Color Blending
        configInfo.colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        //4 - Depth Buffer - keeps track of the depth value for each pixel
        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {}; 
        configInfo.depthStencilInfo.back = {};

        return configInfo;
    }

    void VKPipeline::Bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
    }
}