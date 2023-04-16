#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "platform/EngineWindow.h"
#include "platform/WindowsInput.h"

#include "rendering/OpenGL/OpenGL_Renderer.h"
#include "rendering/OpenGl/OpenGL_Model.h"

namespace LG
{
    glm::vec3 cubePositions[10] = {
        glm::vec3(0.0f,   1.0f,  0.0f),
        glm::vec3(2.0f,   6.0f, -15.0f),
        glm::vec3(-1.5f, -1.2f, -2.5f),
        glm::vec3(-3.8f, -1.0f, -12.3f),
        glm::vec3(2.4f,   1.4f, -3.5f),
        glm::vec3(-1.7f,  4.0f, -7.5f),
        glm::vec3(1.3f,  -1.0f, -2.5f),
        glm::vec3(1.5f,   3.0f, -2.5f),
        glm::vec3(1.5f,   1.2f, -1.5f),
        glm::vec3(-1.3f,  0.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[4] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    void OpenGLRenderer::Init()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LG_CORE_ERROR("Failed to initialize OpenGL context");
        }

        m_model = std::make_unique<Model>("external/engine/models/backpack/backpack.obj");

        glViewport(0, 0, LG::ServiceLocator::GetWindow()->GetWidth(), LG::ServiceLocator::GetWindow()->GetHeight());
        m_camera.SetViewTarget(glm::vec3(1.f, -7.f, -7.f), glm::vec3(0.f, 3.f, 0.0f));
        m_camera.SetPerspectiveProjection(glm::radians(45.f), LG::ServiceLocator::GetWindow()->GetWidth() / LG::ServiceLocator::GetWindow()->GetHeight(), 0.1f, 10000.f);
        
        m_shader = std::make_shared<Shader>("external/engine/shaders/modelVertex.glsl", "external/engine/shaders/modelFragment.glsl");
        m_screenShader = std::make_shared<Shader>("external/engine/shaders/screenVertex.glsl", "external/engine/shaders/screenFragment.glsl");

        m_screenShader->Use();
        m_screenShader->setInt("screenTexture", 2); //TODO: Improve texture Class

        m_screenBuffer = std::make_unique<FrameBuffer>(LG::ServiceLocator::GetWindow()->GetWidth(), LG::ServiceLocator::GetWindow()->GetHeight());

    }
    
    void OpenGLRenderer::Shutdown()
    {
    }

    void OpenGLRenderer::RenderFrame()
    {
        // Render
        // Clear the colorbuffer
        m_shader->Use();
        m_screenBuffer->Bind();
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        World::TransformComponent transformComp;
        transformComp.Translate(glm::vec3(-0.0f, 0.0f, -2000000.0f));
        transformComp.Rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        
        m_shader->setMatrix("model", transformComp.GetTransform());
        m_shader->setMatrix("view", m_camera.GetView());
        m_shader->setMatrix("projection", m_camera.GetProjection());
        m_model->Draw(*m_shader);

        m_screenBuffer->Unbind();
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        m_screenShader->Use();
    }

    void OpenGLRenderer::RendererWaitIdle()
    {
    }

    void OpenGLRenderer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<AppTickEvent>(LG_BIND_EVENT_FN(OpenGLRenderer::OnAppTickEvent));
        dispatcher.Dispatch<MouseMovedEvent>(LG_BIND_EVENT_FN(OpenGLRenderer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(LG_BIND_EVENT_FN(OpenGLRenderer::OnMouseScrolledEvent));

    }

    bool OpenGLRenderer::OnAppTickEvent(AppTickEvent& event)
    {
        m_camera.OnAppTickEvent(event);
        return true;
    }
    bool OpenGLRenderer::OnMouseMovedEvent(MouseMovedEvent& event)
    {
        if (!event.Handled()) 
        {
            m_camera.OnMouseMovedEvent(event);
            return true;
        }
        return false;
    }
    bool OpenGLRenderer::OnMouseScrolledEvent(MouseScrolledEvent& event)
    {
        m_camera.OnMouseScrolledEvent(event);
        return false;
    }
} // namespace LG

