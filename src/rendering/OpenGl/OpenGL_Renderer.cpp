#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "platform/EngineWindow.h"
#include "platform/WindowsInput.h"

#include "rendering/OpenGL/OpenGL_Renderer.h"

namespace LG
{

    static Pyramid  pyramid;
    static Cube     cube;
    static Quad     quad;
    static Triangle triangle;


    glm::vec3 cubePositions[10] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    void OpenGLRenderer::Init(RendererSettings settings)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LG_CORE_ERROR("Failed to initialize OpenGL context");
        }

        glEnable(GL_DEPTH_TEST);


        glViewport(0, 0, LG::ServiceLocator::GetWindow()->GetWidth(), LG::ServiceLocator::GetWindow()->GetHeight());
        m_camera.SetViewTarget(glm::vec3(1.f, -7.f, -7.f), glm::vec3(0.f, 3.f, 0.0f));
        m_camera.SetPerspectiveProjection(glm::radians(45.f), LG::ServiceLocator::GetWindow()->GetWidth() / LG::ServiceLocator::GetWindow()->GetHeight(), 0.1f, 10000.f);
        
        m_shader = std::make_shared<Shader>("external/engine/shaders/vertex.glsl", "external/engine/shaders/fragment.glsl");
        m_buffer = std::make_unique<Buffer>(cube);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(const auto& texture : m_textures)
        {
            texture->Bind();
        }
        //Draw 10 cubes
        for (unsigned int i = 0; i < 10; i++)
        {
            m_transform = World::TransformComponent();
            m_transform.Translate(cubePositions[i]);
            m_transform.Rotate(20.f * i, { 1.0f, 0.3f, 0.5f });
            auto projection = m_camera.GetProjection() * m_camera.GetView();
            glm::mat4 transform = projection * m_transform.GetTransform();
            m_shader->setMatrix(m_shader->GetLocation("transform"), transform);
            m_buffer->Draw(m_shader);
        }

    }

    void OpenGLRenderer::RendererWaitIdle()
    {
    }

    void OpenGLRenderer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<AppTickEvent>(LG_BIND_EVENT_FN(OpenGLRenderer::OnAppTickEvent));

    }

    bool OpenGLRenderer::OnAppTickEvent(AppTickEvent& event)
    {
        //Update Transform here for quick results
        //m_transform.Rotate(90.f * event.GetFixedTick(), World::Axis::Z);
        //m_transform.Rotate(-90.f * event.GetFixedTick(), World::Axis::Y);
        ////m_transform.Rotate(90.f * event.GetFixedTick(), World::Axis::X);
        return true;
    }
} // namespace LG

