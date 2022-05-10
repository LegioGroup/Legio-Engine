#include <Legio/platform/Application.h>
#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>
#include <Legio/platform/Window.h>
#include <Legio/application/Events/Event.h>


#include "WindowsInput.h"
#include "rendering/vulkan/VKRenderer.h"

namespace LG
{
    Application::Application(std::string appName)
        : m_appName(std::move(appName))
    {
        InitServices();
    }

    Application::~Application()
    {
        LG_CORE_INFO("Application {0} deleted!", m_appName);
        ShutdownServices();
    }
    
    void Application::Run()
    {
        while (m_running)
        {
            if(LG::ServiceLocator::GetWindow()->Update())
            {
                m_running = false;
                continue;
            }

            m_clock.InitUpdatesToProcess();
            while(m_clock.ProcessUpdates())
            {
                Update(m_clock.GetFixedTick());
            }

            //Draw Frame
            LG::ServiceLocator::GetRenderer()->RenderFrame();
        }

        LG::ServiceLocator::GetRenderer()->RendererWaitIdle();
    }

    void Application::InitServices()
    {
        ServiceLocator::Provide(new Log());
        ServiceLocator::Provide(new EngineWindow({800, 600, m_appName.c_str()}));
        ServiceLocator::Provide(new WindowsInput());
        ServiceLocator::Provide(new VKRenderer());

        ServiceLocator::GetWindow()->SetEventCallback(LG_BIND_EVENT_FN(Application::OnEvent));

        ServiceLocator::GetLog()->Init();
        ServiceLocator::GetWindow()->Init();
        ServiceLocator::GetInput()->Init();
        ServiceLocator::GetRenderer()->Init(RendererSettings());

        LG_CORE_INFO("Services Initiated!");
    }

    void Application::ShutdownServices()
    {
        LG_CORE_INFO("Shutdown Services!");
        ServiceLocator::ShutdownServices();
    }

    void Application::OnEvent(Event& event)
    {
        LG_CORE_INFO("Event: {0}", event.ToString());
    }

} //namespace LG
