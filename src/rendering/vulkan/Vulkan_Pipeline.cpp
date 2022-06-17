#include "rendering/vulkan/Vulkan_Implementation.h"
#include "rendering/vulkan/Vulkan_Pipeline.h"
#include "rendering/vulkan/Vulkan_Model.h"
#include "rendering/vulkan/Vulkan_Device.h"
#include <fstream>

namespace LG
{
    VKPipeline::VKPipeline(
        VKDevice* device,
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& info
    )
        : m_device(device)
    {
        CreateGraphicsPipeline(vertFilepath, fragFilepath, info);
    }

    VKPipeline::~VKPipeline()
    {
        vkDestroyShaderModule(m_device->GetDevice(), m_fragmentShaderModule, nullptr);
        vkDestroyShaderModule(m_device->GetDevice(), m_vertexShaderModule, nullptr);
        vkDestroyPipeline(m_device->GetDevice(), m_graphicsPipeline, nullptr);
    }

    std::vector<char> VKPipeline::ReadFile(const std::string& fileName)
    {
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);

        if(!file.is_open())
        {
            throw std::runtime_error("Failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    void VKPipeline::CreateGraphicsPipeline(
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& info
    )
    {
        assert(info.pipelineLayout != VK_NULL_HANDLE && "Cannot Create Graphics Pipeline: no pipelineLayout Provided in configInfo");
        assert(info.renderPass != VK_NULL_HANDLE && "Cannot Create Graphics Pipeline: no renderPass Provided in configInfo");

        auto vertShaderCode = ReadFile(vertFilepath);
        auto fragShaderCode = ReadFile(fragFilepath);

        CreateShaderModule(vertShaderCode, &m_vertexShaderModule); 
        CreateShaderModule(fragShaderCode, &m_fragmentShaderModule); 

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = m_vertexShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = m_fragmentShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        auto bindingDescription = VKModel::Vertex::GetBindingDescription();
        auto attributesDescription = VKModel::Vertex::GetAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributesDescription.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributesDescription.data(); // Optional


        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &info.inputAssemblyInfo;
        pipelineInfo.pViewportState = &info.viewportState;
        pipelineInfo.pRasterizationState = &info.rasterizationInfo;
        pipelineInfo.pMultisampleState = &info.multisampleInfo;
        pipelineInfo.pColorBlendState = &info.colorBlending;
        pipelineInfo.pDepthStencilState = &info.depthStencilInfo;
        pipelineInfo.pDynamicState = &info.dynamicState;

        pipelineInfo.layout = info.pipelineLayout;
        pipelineInfo.renderPass = info.renderPass;
        pipelineInfo.subpass = info.subpass;
        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create Graphics Pipeline");
        }
    }

    void VKPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
    {
        // 1 - Input Assembly
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        //4 - Combine Viewport and Scissors
        configInfo.viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.viewportState.viewportCount = 1;
        configInfo.viewportState.pViewports = nullptr;
        configInfo.viewportState.scissorCount = 1;
        configInfo.viewportState.pScissors = nullptr;

        //5 - Rasterizer
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f; // Optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

        //6 - MultiSampling
        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f; // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr; // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

        //7 - Depth Stencil
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

        //8 - Color Blend
        configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        configInfo.colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlending.logicOpEnable = VK_FALSE;
        configInfo.colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        configInfo.colorBlending.attachmentCount = 1;
        configInfo.colorBlending.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlending.blendConstants[0] = 0.0f; // Optional
        configInfo.colorBlending.blendConstants[1] = 0.0f; // Optional
        configInfo.colorBlending.blendConstants[2] = 0.0f; // Optional
        configInfo.colorBlending.blendConstants[3] = 0.0f; // Optional

        configInfo.dyamicStateEnables = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        configInfo.dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.dynamicState.dynamicStateCount = static_cast<uint32_t>(configInfo.dyamicStateEnables.size());
        configInfo.dynamicState.pDynamicStates = configInfo.dyamicStateEnables.data();
        configInfo.dynamicState.flags = 0;
    }
    
    void VKPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(m_device->GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }
    }

    void VKPipeline::Bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
    }
} // namespace LG
