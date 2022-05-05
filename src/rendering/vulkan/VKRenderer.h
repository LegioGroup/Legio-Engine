#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include "rendering/vulkan/VKPipeline.h"
namespace LG
{
    class VKRenderer : public Renderer
    {
    public:
        VKRenderer() = default;
        virtual ~VKRenderer();

        virtual void Init(RendererSettings settings) override;
        virtual void Shutdown() override;
        virtual void RenderFrame()  override;
    
    private:
        EngineWindow* m_engineWindow = nullptr;
        std::unique_ptr<VKDevice> m_device = nullptr;
        std::unique_ptr<VKPipeline> m_pipeline = nullptr;
    };
} // namespace LG
