#include "EngineWindow.h"
#include <Legio/platform/Log.h>
namespace LG
{

    EngineWindow::EngineWindow(WindowProps windowSettings)
    {
        m_data.m_height = windowSettings.m_height;
        m_data.m_width = windowSettings.m_width;
        m_data.m_name = std::move(windowSettings.m_name);
    }

    EngineWindow::~EngineWindow()
    {
        LG_CORE_INFO("Shutting Down Engine Window");
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void EngineWindow::FrameBufferResizedCB(GLFWwindow* window, int width, int height)
    {

    }

    void EngineWindow::Init()
    {
        if(!glfwInit())
        {
            LG_CORE_CRITICAL("Couldn't initialize GLFW!");
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_name, nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, &m_data);
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.m_width = width;
            data.m_height = height;

            FrameBufferResizeEvent event(width, height);
            data.m_eventCallbackFn(event);
        });

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.m_width = width;
            data.m_height = height;

            WindowResizeEvent event(width, height);
            data.m_eventCallbackFn(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.m_eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.m_eventCallbackFn(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.m_eventCallbackFn(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(keycode);
            data.m_eventCallbackFn(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.m_eventCallbackFn(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.m_eventCallbackFn(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data.m_eventCallbackFn(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
            data.m_eventCallbackFn(event);
        });
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

    VkExtent2D EngineWindow::GetExtent() const
    {
        return {static_cast<uint32_t>(m_data.m_width), static_cast<uint32_t>(m_data.m_height)};
    }
} //namespace LG