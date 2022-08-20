#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <Legio/rendering/LGCamera.h>
#include <optional>
#include <memory.h>
namespace LG
{
    class OpenGLRenderer : public Renderer
    {
        virtual void Init(RendererSettings settings){};
        virtual void Shutdown() override {};
        virtual void RenderFrame() override {};
        virtual void RendererWaitIdle() override {};
        virtual void OnEvent(Event& event) override {};
    };
} //namespace LG