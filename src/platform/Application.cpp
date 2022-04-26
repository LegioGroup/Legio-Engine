#include <Legio/platform/Application.h>
#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>
#include <Legio/platform/Window.h>
#include "EngineWindow.h"

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
        ServiceLocator::GetWindow()->OpenWindow();

        while (m_running)
        {
            if(LG::ServiceLocator::GetWindow()->Update())
            {
                m_running = false;
                continue;
            }

            Update(0.0f);
        }
    }

    void Application::InitServices()
    {
        ServiceLocator::Provide(new Log());
        ServiceLocator::Provide(new EngineWindow());

        ServiceLocator::GetLogger()->Init();
        LG_CORE_INFO("Services Initiated!");
    }

    void Application::ShutdownServices()
    {
        LG_CORE_INFO("Shutdown Services!");
        ServiceLocator::ShutdownServices();
    }

} //namespace LG
