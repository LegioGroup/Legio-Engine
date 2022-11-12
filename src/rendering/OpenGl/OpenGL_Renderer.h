#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <Legio/rendering/LGCamera.h>
#include <Legio/world/components/TransformComponent.h>

#include <glad/glad.h>
#include <optional>
#include <memory.h>
#include <rendering/OpenGL/OpenGL_Shader.h>
#include <rendering/OpenGL/OpenGL_Buffer.h>
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
        virtual unsigned int GetRenderTexture() const { return m_renderTexture; };
    private:
        bool OnAppTickEvent(AppTickEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);
    private:
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Shader> m_screenShader;
        std::unique_ptr<Buffer> m_buffer;
        std::vector<std::shared_ptr<Texture>> m_textures;

        //Temp
        LGCamera m_camera;
        World::TransformComponent m_camTransform;
        unsigned int m_fbo;
        unsigned int m_rbo;
        unsigned int m_renderTexture;
        unsigned int m_quadVAO, m_quadVBO;

    };
} //namespace LG