#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "rendering/vulkan/VKRenderer.h"
namespace LG
{
    VKRenderer::~VKRenderer()
    {
        LG_CORE_INFO("Shutting Down Vulkan Renderer!");
        vkDestroyPipelineLayout(m_device->GetDevice(), m_pipelineLayout, nullptr);
    }

    void VKRenderer::Init(RendererSettings settings)
    {
        m_engineWindow = static_cast<EngineWindow*>(ServiceLocator::GetWindow().get());
        m_device = std::make_unique<VKDevice>(*m_engineWindow);
        m_swapChain = std::make_unique<VKSwapChain>(*m_device, m_engineWindow->GetExtent());

        CreatePipelineLayout();
        CreatePipeline();
        CreateCommandBuffers();

        LG_CORE_INFO("Initialize Vulkan Renderer!"); 
    }

    void VKRenderer::Shutdown()
    {
    }

    void VKRenderer::RenderFrame()
    {

    }

    void VKRenderer::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(m_device->GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Pipeline Layout");
        }
    }

    void VKRenderer::CreatePipeline()
    {
        auto pipelineConfig = VKPipeline::DefaultPipelineConfigInfo(m_swapChain->Width(), m_swapChain->Height());

        pipelineConfig.renderPass = m_swapChain->GetRenderPass();
        pipelineConfig.pipelineLayout = m_pipelineLayout;
        m_pipeline = std::make_unique<VKPipeline>(  
            *m_device,
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert.spv",
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag.spv",
            pipelineConfig
        );
    }

    void VKRenderer::CreateCommandBuffers()
    {

    }

} // namespace LG
