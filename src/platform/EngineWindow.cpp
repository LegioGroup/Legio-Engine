#include "EngineWindow.h"
#include <Legio/platform/Log.h>
namespace LG
{
    EngineWindow::~EngineWindow()
    {
        LG_CORE_INFO("Shutting Down Engine Window");
        glfwDestroyWindow(m_window);
    }

    void EngineWindow::OpenWindow()
    {
        if(!glfwInit())
        {
            LG_CORE_CRITICAL("Couldn't initialize GLFW!");
        }
        m_window = glfwCreateWindow(800, 600, "Legio Engine", nullptr, nullptr);
    }

    bool EngineWindow::Update()
    {
        glfwPollEvents();

        return glfwWindowShouldClose(m_window);
    }
} //namespace LG