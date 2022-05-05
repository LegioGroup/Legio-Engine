#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
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

    };
} // namespace LG
