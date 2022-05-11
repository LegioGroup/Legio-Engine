#pragma once
#include <Legio/platform/Window.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
namespace LG
{
    class EngineWindow : public Window
    {
    public:
        EngineWindow(WindowProps windowSettings = WindowProps());
        virtual ~EngineWindow();
        virtual void Init() override;
        virtual void Update() override;

        inline GLFWwindow* GetNativeWindow() const { return m_window; }
        virtual void SetEventCallback(const EventCallbackFn& callback) { m_data.m_eventCallbackFn = callback; };
        //Vulkan Specific
        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        VkExtent2D GetExtent();

    private:
        GLFWwindow* m_window = nullptr;

        struct WindowData
        {
            const char* m_name;
            int m_width;
            int m_height;
            EventCallbackFn m_eventCallbackFn;
        };

        WindowData m_data;

    };
} //namespace LG
