#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <Legio/rendering/LGCamera.h>

#include <glad/glad.h>
#include <optional>
#include <memory.h>
#include <rendering/OpenGL/OpenGL_Shader.h>
#include <rendering/OpenGL/OpenGL_Buffer.h>
namespace LG
{
    class OpenGLRenderer : public Renderer
    {
    public:
        virtual void Init(RendererSettings settings) override;
        virtual void Shutdown() override;
        virtual void RenderFrame() override;
        virtual void RendererWaitIdle() override;
        virtual void OnEvent(Event& event) override;

    private:
        std::shared_ptr<Shader> m_shader;
        std::unique_ptr<Buffer> m_buffer;
    };
} //namespace LG