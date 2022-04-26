#pragma once 
#include <glm/glm.hpp>
#include <Legio/ServiceLocator.h>

#include <Legio/platform/Application.h>

int main(int argc, char** argv)
{
    //Create App
    LG::Application* app = CreateApplication();

    //Run App
    app->Run();

    delete app;
}