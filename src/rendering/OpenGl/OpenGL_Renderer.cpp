#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "platform/EngineWindow.h"
#include "platform/WindowsInput.h"

#include "rendering/OpenGL/OpenGL_Renderer.h"

namespace LG
{
    std::vector<Vertex> vertices = {
        { glm::vec3( 0.0f,  0.5f, 0.0f)},
        { glm::vec3(-0.5f, -0.5f, 0.0f)},
        { glm::vec3( 0.5f, -0.5f, 0.0f)}
    };

    std::vector<uint32_t> indices = {
        0, 1, 2	//Triangle 1
    };


    void OpenGLRenderer::Init(RendererSettings settings)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LG_CORE_ERROR("Failed to initialize OpenGL context");
        }

        glViewport(0, 0, LG::ServiceLocator::GetWindow()->GetWidth(), LG::ServiceLocator::GetWindow()->GetHeight());
        
        m_shader = std::make_shared<Shader>("external/engine/shaders/vertex.glsl", "external/engine/shaders/fragment.glsl");
        m_buffer = std::make_unique<Buffer>(vertices, indices, m_shader);
        //glGenVertexArrays(1, &VAO);
        //glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);

        //glBindVertexArray(VAO);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //m_shader->SetupAttribs();
        //glEnableVertexAttribArray(0);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);

    }
    
    void OpenGLRenderer::Shutdown()
    {
        //glDeleteVertexArrays(1, &VAO);
        //glDeleteBuffers(1, &VBO);
        //glDeleteBuffers(1, &EBO);
    }

    void OpenGLRenderer::RenderFrame()
    {
        // Render
        // Clear the colorbuffer

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_buffer->Draw();
        //m_shader->Use();
        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void OpenGLRenderer::RendererWaitIdle()
    {

    }

    void OpenGLRenderer::OnEvent(Event& event)
    {

    }
} // namespace LG

