#include "EngineWindow.h"
#include <Legio/platform/Log.h>
namespace LG
{

    EngineWindow::EngineWindow(WindowSettings windowSettings)
        : Window(windowSettings)
    {

    }

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

     void EngineWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
     {
         if(glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
         {
             LG_CORE_CRITICAL("Failed to create Window Surface");
             throw std::runtime_error("Failed to create Window Surface");
         }
     }
} //namespace LG