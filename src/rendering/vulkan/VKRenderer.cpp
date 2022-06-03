#include <array>
#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>
#include "platform/WindowsInput.h"

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
    }

    void VKRenderer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<FrameBufferResizeEvent>(LG_BIND_EVENT_FN(VKRenderer::OnFrameBufferResizeEvent));
        dispatcher.Dispatch<AppTickEvent>(LG_BIND_EVENT_FN(VKRenderer::OnAppTickEvent));
    }

    bool VKRenderer::OnAppTickEvent(AppTickEvent& event)
    {
        float fixedTick = event.GetFixedTick();

        return false;
    }

    bool VKRenderer::OnFrameBufferResizeEvent(FrameBufferResizeEvent& e)
    {
        return false;
    }

    void VKRenderer::RenderFrame()
    {
        
    }

    void VKRenderer::RendererWaitIdle()
    {
    }
} // namespace LG
