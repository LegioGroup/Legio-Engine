#include "GLFWwindow.h"

namespace LG
{
    void CustomWindow::OpenWindow()
    {
        glfwInit();
        m_window = glfwCreateWindow(800, 600, "Legio Engine", nullptr, nullptr);
    }

    bool CustomWindow::Update()
    {
        glfwPollEvents();

        return glfwWindowShouldClose(m_window);
    }
} //namespace LG