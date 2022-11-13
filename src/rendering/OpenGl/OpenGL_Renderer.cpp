#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "platform/EngineWindow.h"
#include "platform/WindowsInput.h"

#include "rendering/OpenGL/OpenGL_Renderer.h"

namespace LG
{

    static Pyramid          pyramid;
    static Cube             cube;
    static Quad             quad;
    static Triangle         triangle;
    static CubeNoIndices    cubeNoIndices;


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

        glViewport(0, 0, LG::ServiceLocator::GetWindow()->GetWidth(), LG::ServiceLocator::GetWindow()->GetHeight());
        m_camera.SetViewTarget(glm::vec3(1.f, -7.f, -7.f), glm::vec3(0.f, 3.f, 0.0f));
        m_camera.SetPerspectiveProjection(glm::radians(45.f), LG::ServiceLocator::GetWindow()->GetWidth() / LG::ServiceLocator::GetWindow()->GetHeight(), 0.1f, 10000.f);
        
        m_shader = std::make_shared<Shader>("external/engine/shaders/vertex.glsl", "external/engine/shaders/fragment.glsl");
        m_screenShader = std::make_shared<Shader>("external/engine/shaders/screenVertex.glsl", "external/engine/shaders/screenFragment.glsl");
        m_sourceLightCubeShader = std::make_shared<Shader>("external/engine/shaders/sourceLightCubeVertex.glsl", "external/engine/shaders/sourceLightCubeFragment.glsl");
        m_lightShader = std::make_shared<Shader>("external/engine/shaders/multipleLightsVertex.glsl", "external/engine/shaders/multipleLightsFragment.glsl");


        m_buffer = std::make_unique<Buffer>(cubeNoIndices);
        m_textures.emplace_back(Texture::Load("external/engine/models/textures/front.png"));
        m_textures.emplace_back(Texture::Load("external/engine/models/textures/awesomeface.png"));
        m_textures.emplace_back(Texture::Load("external/engine/models/textures/container2.png"));
        m_textures.emplace_back(Texture::Load("external/engine/models/textures/container2_specular.png"));
        m_textures.emplace_back(Texture::Load("external/engine/models/textures/matrix.jpg"));

        m_ligtObjectBuffer = std::make_unique<Buffer>(pyramid);

        m_shader->Use();
        m_shader->setInt("fTexture1", m_textures[0]->GetID());
        m_shader->setInt("fTexture2", m_textures[1]->GetID());

        m_lightShader->Use();
        m_lightShader->setInt("material.diffuse", m_textures[2]->GetID());
        m_lightShader->setInt("material.specular", m_textures[3]->GetID());

        //Directional Light
        m_lightShader->setVec3("dirLight.direction", { -0.2f, -1.0f, -0.3f });
        m_lightShader->setVec3("dirLight.ambient", {0.05f, 0.05f, 0.05f});
        m_lightShader->setVec3("dirLight.diffuse", {0.4f, 0.4f, 0.4f});
        m_lightShader->setVec3("dirLight.specular", {0.5f, 0.5f, 0.5f});

        // point light 1
        m_lightShader->setVec3("pointLights[0].position", pointLightPositions[0]);
        m_lightShader->setVec3("pointLights[0].ambient", { 0.05f, 0.05f, 0.05f });
        m_lightShader->setVec3("pointLights[0].diffuse", {0.8f, 0.8f, 0.8f});
        m_lightShader->setVec3("pointLights[0].specular", { 1.0f, 1.0f, 1.0f });
        m_lightShader->setFloat("pointLights[0].constant", 1.0f);
        m_lightShader->setFloat("pointLights[0].linear", 0.09f);
        m_lightShader->setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        m_lightShader->setVec3("pointLights[1].position", pointLightPositions[1]);
        m_lightShader->setVec3("pointLights[1].ambient", { 0.05f, 0.05f, 0.05f });
        m_lightShader->setVec3("pointLights[1].diffuse", {0.8f, 0.8f, 0.8f});
        m_lightShader->setVec3("pointLights[1].specular", { 1.0f, 1.0f, 1.0f });
        m_lightShader->setFloat("pointLights[1].constant", 1.0f);
        m_lightShader->setFloat("pointLights[1].linear", 0.09f);
        m_lightShader->setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        m_lightShader->setVec3("pointLights[2].position", pointLightPositions[2]);
        m_lightShader->setVec3("pointLights[2].ambient", { 0.05f, 0.05f, 0.05f });
        m_lightShader->setVec3("pointLights[2].diffuse", { 0.8f, 0.8f, 0.8f });
        m_lightShader->setVec3("pointLights[2].specular", {1.0f, 1.0f, 1.0f});
        m_lightShader->setFloat("pointLights[2].constant", 1.0f);
        m_lightShader->setFloat("pointLights[2].linear", 0.09f);
        m_lightShader->setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        m_lightShader->setVec3("pointLights[3].position", pointLightPositions[3]);
        m_lightShader->setVec3("pointLights[3].ambient", { 0.05f, 0.05f, 0.05f });
        m_lightShader->setVec3("pointLights[3].diffuse", {0.8f, 0.8f, 0.8f});
        m_lightShader->setVec3("pointLights[3].specular", { 1.0f, 1.0f, 1.0f });
        m_lightShader->setFloat("pointLights[3].constant", 1.0f);
        m_lightShader->setFloat("pointLights[3].linear", 0.09f);
        m_lightShader->setFloat("pointLights[3].quadratic", 0.032f);

        // spotLight
        m_lightShader->setVec3("spotLight.position", {});
        m_lightShader->setVec3("spotLight.direction", {});
        m_lightShader->setVec3("spotLight.ambient", { 0.0f, 0.0f, 0.0f });
        m_lightShader->setVec3("spotLight.diffuse", { 1.0f, 1.0f, 1.0f });
        m_lightShader->setVec3("spotLight.specular", { 1.0f, 1.0f, 1.0f });
        m_lightShader->setFloat("spotLight.constant", 1.0f);
        m_lightShader->setFloat("spotLight.linear", 0.09f);
        m_lightShader->setFloat("spotLight.quadratic", 0.032f);
        m_lightShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        m_lightShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        //m_lightShader->setInt("material.emission", m_textures[4]->GetID());
        m_lightShader->setVec3("light.ambient", { 0.2f, 0.2f, 0.2f });
        m_lightShader->setVec3("light.diffuse", {0.5f, 0.5f, 0.5f}); // darken diffuse light a bit
        m_lightShader->setVec3("light.specular", { 1.0f, 1.0f, 1.0f });

        m_sourceLightCubeShader->Use();

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

        m_screenBuffer->Bind();
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(const auto& texture : m_textures)
        {
            texture->Bind();
        }

        //Objects---------------------------Draw 10 cubes
        for (unsigned int i = 0; i < 10; i++)
        {
            World::TransformComponent transformComp;
            transformComp.Translate(cubePositions[i]);
            transformComp.Rotate(20.f * i, { 1.0f, 0.3f, 0.5f });

            //glm::vec3 lightColor;
            //lightColor.x = sin(glfwGetTime() * 2.0f);
            //lightColor.y = sin(glfwGetTime() * 0.7f);
            //lightColor.z = sin(glfwGetTime() * 1.3f);

            //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
            //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

            //m_lightShader->setVec3("light.ambient", ambientColor);
            //m_lightShader->setVec3("light.diffuse", diffuseColor);

            m_lightShader->setVec3("spotLight.position", m_camera.GeTransform().GetPositon());
            m_lightShader->setVec3("spotLight.direction", m_camera.GetCameraFront());
            m_lightShader->setVec3("viewPos", m_camera.GeTransform().GetPositon());
            m_lightShader->setMatrix("model", transformComp.GetTransform());
            m_lightShader->setMatrix("view", m_camera.GetView());
            m_lightShader->setMatrix("projection", m_camera.GetProjection());
            m_lightShader->setVec3("material.ambient", { 1.0f, 0.5f, 0.31f });
            m_lightShader->setVec3("material.diffuse", {1.0f, 0.5f, 0.31f});
            m_lightShader->setVec3("material.specular", { 0.5f, 0.5f, 0.5f });
            m_lightShader->setFloat("material.shininess", 64.0f);

            m_buffer->Draw(m_lightShader);
        }
        //---------------------------------------
        //Light---------------------------
        for (size_t i = 0; i < 4; i++)
        {
            m_sourceLightCubeShader->Use();
            //transformComp.Rotate(20.f, { 1.2f, -3.0f, 2.0f });
            World::TransformComponent transformComponent;
            transformComponent.Translate(pointLightPositions[i]);
            transformComponent.Scale({ .25f,.25f,.25f });
            auto projection = m_camera.GetProjection() * m_camera.GetView();
            glm::mat4 transform = projection * transformComponent.GetTransform();
            m_sourceLightCubeShader->setMatrix("transform", transform);
            m_ligtObjectBuffer->Draw(m_sourceLightCubeShader);
        }

        //---------------------------------------

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

