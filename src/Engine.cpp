#include <Legio/Engine.h>
#include <iostream>
#include <Legio/ServiceLocator.h>

#include "platform/GLFWwindow.h"
namespace LG
{
    void Engine::Init()
    {

        ServiceLocator::Provide(new Log());
        ServiceLocator::GetLogger()->Init();
        
        ServiceLocator::Provide(new CustomWindow());     

        LG_CORE_CRITICAL("Critical {0} - {1} - {2}", 10, 20, 30);
        LG_CORE_ERROR("error");
        LG_CORE_WARN("warning");
        LG_CORE_INFO("info");
        LG_CORE_TRACE("trace");

        LG_APP_CRITICAL("Critical {0} - {1} - {2}", 10, 20, 30);
        LG_APP_ERROR("error");
        LG_APP_WARN("warning");
        LG_APP_INFO("info");
        LG_APP_TRACE("trace");
    }
} //namespace LG