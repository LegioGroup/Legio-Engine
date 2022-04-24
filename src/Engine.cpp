#include <Legio/Engine.h>
#include <iostream>
#include <Legio/ServiceLocator.h>

#include "platform/GLFWwindow.h"
namespace LG
{
    void Engine::Init()
    {
        std ::cout << "Init Window\n";

        ServiceLocator::Proivide(new CustomWindow());       
    }
} //namespace LG