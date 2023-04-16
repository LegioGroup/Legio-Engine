#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <Legio/rendering/LGCamera.h>
#include <Legio/world/components/TransformComponent.h>

#include <glad/glad.h>
#include <optional>
#include <memory>
#include <rendering/OpenGL/OpenGL_Shader.h>
//TODO: Unify buffers
#include <rendering/OpenGL/OpenGL_Buffer.h>
#include <rendering/OpenGL/OpenGL_FrameBuffer.h>
#include <rendering/OpenGL/OpenGL_Texture.h>
#include <rendering/OpenGl/OpenGL_Model.h>

namespace LG
{
    class OpenGLRenderer : public Renderer
    {
    public:
        void Init() override;
        void Shutdown() override;
        void RenderFrame() override;
        void RendererWaitIdle() override;
        void OnEvent(Event& event) override;
        unsigned int GetRenderTexture() const override{ return m_screenBuffer->GetFrameTexture(); };
        FrameBuffer* GetViewportScreenBuffer() override { return m_screenBuffer.get(); }
    private:
        bool OnAppTickEvent(AppTickEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);

    private:
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Shader> m_screenShader;

        std::unique_ptr<Model> m_model;

        std::unique_ptr<FrameBuffer> m_screenBuffer;
        //Temp
        LGCamera m_camera;

    };
} //namespace LG