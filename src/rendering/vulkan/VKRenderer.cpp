#include "rendering/vulkan/VKRenderer.h"

namespace LG
{
    VKRenderer::~VKRenderer()
    {
        LG_CORE_INFO("Shutting Down Vulkan Renderer!");
    }

    void VKRenderer::Init(RendererSettings settings)
    {
         LG_CORE_INFO("Initialize Vulkan Renderer!"); 
    }

    void VKRenderer::Shutdown()
    {
    };

    void VKRenderer::RenderFrame(){};
} // namespace LG
