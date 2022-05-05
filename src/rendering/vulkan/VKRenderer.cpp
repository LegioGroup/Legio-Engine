#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "rendering/vulkan/VKRenderer.h"
namespace LG
{
    VKRenderer::~VKRenderer()
    {
        LG_CORE_INFO("Shutting Down Vulkan Renderer!");
        m_pipeline.reset();
        m_device.reset();
    }

    void VKRenderer::Init(RendererSettings settings)
    {
        m_engineWindow = static_cast<EngineWindow*>(ServiceLocator::GetWindow().get());
        m_device = std::make_unique<VKDevice>(*m_engineWindow);
        m_pipeline = std::make_unique<VKPipeline>(  
            *m_device,
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert.spv",
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag.spv",
            VKPipeline::DefaultPipelineConfigInfo(m_engineWindow->GetWindowSettings().m_width, m_engineWindow->GetWindowSettings().m_height)
        );

        LG_CORE_INFO("Initialize Vulkan Renderer!"); 
    }

    void VKRenderer::Shutdown()
    {
    };

    void VKRenderer::RenderFrame(){};
} // namespace LG
