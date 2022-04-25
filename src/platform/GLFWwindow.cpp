#include "GLFWwindow.h"
#include <Legio/platform/Log.h>
namespace LG
{
    void CustomWindow::OpenWindow()
    {
        if(!glfwInit())
        {
            LG_CORE_CRITICAL("Couldn't initialize GLFW!");
        }
        m_window = glfwCreateWindow(800, 600, "Legio Engine", nullptr, nullptr);
    }

    bool CustomWindow::Update()
    {
        glfwPollEvents();

        return glfwWindowShouldClose(m_window);
    }
} //namespace LG