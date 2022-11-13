#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <Legio/rendering/LGCamera.h>
#include <Legio/world/components/TransformComponent.h>

#include <glad/glad.h>
#include <optional>
#include <memory.h>
#include <rendering/OpenGL/OpenGL_Shader.h>
//TODO: Unify buffers
#include <rendering/OpenGL/OpenGL_Buffer.h>
#include <rendering/OpenGL/OpenGL_FrameBuffer.h>
#include <rendering/OpenGL/OpenGL_Texture.h>

namespace LG
{
    class OpenGLRenderer : public Renderer
    {
    public:
        virtual void Init() override;
        virtual void Shutdown() override;
        virtual void RenderFrame() override;
        virtual void RendererWaitIdle() override;
        virtual void OnEvent(Event& event) override;
        virtual unsigned int GetRenderTexture() const override{ return m_screenBuffer->GetFrameTexture(); };
        virtual FrameBuffer* GetViewportScreenBuffer() override { return m_screenBuffer.get(); }
    private:
        bool OnAppTickEvent(AppTickEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);
    private:
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Shader> m_screenShader;
        std::shared_ptr<Shader> m_lightShader;
        std::shared_ptr<Shader> m_sourceLightCubeShader;

        std::unique_ptr<Buffer> m_buffer;
        std::unique_ptr<Buffer> m_ligtObjectBuffer;

        std::unique_ptr<FrameBuffer> m_screenBuffer;
        std::vector<std::shared_ptr<Texture>> m_textures;

        //Temp
        LGCamera m_camera;
        World::TransformComponent m_camTransform;

    };
} //namespace LG