#pragma once
#include <Legio/platform/Window.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
namespace LG
{
    class EngineWindow : public Window
    {
    public:
        EngineWindow(WindowSettings windowSettings = WindowSettings());
        ~EngineWindow();
        virtual void OpenWindow(WindowSettings data = WindowSettings()) override;
        virtual bool Update() override;
        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    private:
        GLFWwindow* m_window = nullptr;
    };
} //namespace LG
