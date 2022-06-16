#include "rendering/vulkan/renderSystems/BasicRenderSystem.h"

namespace LG
{
    BasicRenderSystem::BasicRenderSystem(VKDevice* device, VkRenderPass renderPass, LGCamera* camera)
        : m_device(device)
        , m_camera(camera)
    {
        CreatePipelineLayout();
        CreatePipeline(renderPass);
        m_model = new VKModel(m_device);
    }

    BasicRenderSystem::~BasicRenderSystem()
    {
        vkDestroyPipelineLayout(m_device->GetDevice(), m_pipelineLayout, nullptr);
        m_pipeline.reset();
        delete m_model;
    }

    void BasicRenderSystem::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(m_device->GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        } 
    }

    void BasicRenderSystem::CreatePipeline(VkRenderPass renderPass)
    {
        PipelineConfigInfo pipelineConfig = {};
        VKPipeline::DefaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_pipelineLayout;

        m_pipeline = std::make_unique<VKPipeline>(m_device,
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert.spv",
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag.spv",
            pipelineConfig);
    }

    void BasicRenderSystem::Render(VkCommandBuffer commandBuffer)
    {
        m_pipeline->Bind(commandBuffer);

        m_model->Bind(commandBuffer);
        m_model->Draw(commandBuffer);
    }
    
} // namespace LG
