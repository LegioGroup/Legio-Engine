#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "platform/EngineWindow.h"
#include "platform/WindowsInput.h"

#include "rendering/OpenGL/OpenGL_Renderer.h"

namespace LG
{
    std::vector<Vertex> vertices = {
        { glm::vec3( 0.5f,  0.5f, 0.0f), {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {}},
        { glm::vec3( 0.5f, -0.5f, 0.0f), {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}},
        { glm::vec3(-0.5f, -0.5f, 0.0f), {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {}},
        { glm::vec3(-0.5f,  0.5f, 0.0f), {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}}
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,//Triangle 1
        0, 2, 3 //Triangle 2
    };


    void OpenGLRenderer::Init(RendererSettings settings)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LG_CORE_ERROR("Failed to initialize OpenGL context");
        }

        glViewport(0, 0, LG::ServiceLocator::GetWindow()->GetWidth(), LG::ServiceLocator::GetWindow()->GetHeight());
        
        m_shader = std::make_shared<Shader>("external/engine/shaders/vertex.glsl", "external/engine/shaders/fragment.glsl");
        m_buffer = std::make_unique<Buffer>(vertices, indices);
        m_textures.emplace_back(Texture::Load("external/engine/models/textures/front.png"));
        m_textures.emplace_back(Texture::Load("external/engine/models/textures/awesomeface.png"));

        m_shader->Use();
        m_shader->setInt(m_shader->GetLocation("fTexture1"), m_textures[0]->GetID());
        m_shader->setInt(m_shader->GetLocation("fTexture2"), m_textures[1]->GetID());
    }
    
    void OpenGLRenderer::Shutdown()
    {
    }

    void OpenGLRenderer::RenderFrame()
    {
        // Render
        // Clear the colorbuffer

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for(const auto& texture : m_textures)
        {
            texture->Bind();
        }

        m_buffer->Draw(m_shader);
    }

    void OpenGLRenderer::RendererWaitIdle()
    {

    }

    void OpenGLRenderer::OnEvent(Event& event)
    {

    }
} // namespace LG

