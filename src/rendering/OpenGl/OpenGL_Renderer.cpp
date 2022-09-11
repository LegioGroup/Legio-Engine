#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "platform/EngineWindow.h"
#include "platform/WindowsInput.h"

#include "rendering/OpenGL/OpenGL_Renderer.h"

namespace LG
{
    void OpenGLRenderer::Init(RendererSettings settings)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LG_CORE_ERROR("Failed to initialize OpenGL context");
        }

        glViewport(0, 0, LG::ServiceLocator::GetWindow()->GetWidth(), LG::ServiceLocator::GetWindow()->GetHeight());
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

    }

    void OpenGLRenderer::RendererWaitIdle()
    {

    }

    void OpenGLRenderer::OnEvent(Event& event)
    {

    }
} // namespace LG

