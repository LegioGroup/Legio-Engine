#pragma once
#include <Legio/application/Events/Event.h>
namespace LG
{
    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual void RenderFrame() = 0;
        virtual void RendererWaitIdle(){};
        virtual void OnEvent(Event& event) = 0;
    };
}
