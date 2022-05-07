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

    void EngineWindow::FrameBufferResizedCB(GLFWwindow* window, int width, int height)
    {
        auto engineWindow = reinterpret_cast<EngineWindow*>(glfwGetWindowUserPointer(window));
        engineWindow->m_frameBufferResized = true;
        engineWindow->m_windowSettings.m_width = width;
        engineWindow->m_windowSettings.m_height = height;
    }

    void EngineWindow::Init()
    {
        if(!glfwInit())
        {
            LG_CORE_CRITICAL("Couldn't initialize GLFW!");
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow(m_windowSettings.m_width, m_windowSettings.m_height, m_windowSettings.m_name, nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, FrameBufferResizedCB);
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

    VkExtent2D EngineWindow::GetExtent()
    {
        return {static_cast<uint32_t>(m_windowSettings.m_width), static_cast<uint32_t>(m_windowSettings.m_height)};
    }
} //namespace LG