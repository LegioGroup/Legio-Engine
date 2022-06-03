#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
namespace LG
{
    class VKRenderer : public Renderer
    {
    public:
        VKRenderer() = default;
        VKRenderer(const VKRenderer&) = delete;
        VKRenderer& operator=(const VKRenderer&) = delete;

        virtual ~VKRenderer();

        virtual void Init(RendererSettings settings) override;
        virtual void Shutdown() override;
        virtual void RenderFrame() override;
        virtual void RendererWaitIdle() override;
        virtual void OnEvent(Event& event) override;
    private:
        bool OnFrameBufferResizeEvent(FrameBufferResizeEvent& e);
        bool OnAppTickEvent(AppTickEvent& event);
    private:
    };
} // namespace LG
