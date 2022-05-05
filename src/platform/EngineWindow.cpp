#include "EngineWindow.h"
#include <Legio/platform/Log.h>
namespace LG
{
    EngineWindow::~EngineWindow()
    {
        LG_CORE_INFO("Shutting Down Engine Window");
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void EngineWindow::OpenWindow(WindowSettings data)
    {
        if(!glfwInit())
        {
            LG_CORE_CRITICAL("Couldn't initialize GLFW!");
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(data.m_width, data.m_height, data.m_name, nullptr, nullptr);
    }

    bool EngineWindow::Update()
    {
        glfwPollEvents();

        return glfwWindowShouldClose(m_window);
    }
} //namespace LG