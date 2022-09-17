#pragma once
#include <Legio/platform/Window.h>

#ifdef LG_VULKAN_API
    #define GLFW_INCLUDE_VULKAN
#endif // LG_VULKAN_API
#include <GLFW/glfw3.h>
namespace LG
{
    class EngineWindow : public Window
    {
    public:
        EngineWindow(WindowProps windowSettings = WindowProps());
        virtual ~EngineWindow();
        virtual void Init() override;
        virtual bool Update() override;
        virtual int GetWidth() const override { return m_data.m_width;  }
        virtual int GetHeight() const override { return m_data.m_height; }
        virtual void SwapWindowBuffers() override;
        static inline const char** GetRequiredInstanceExtension(uint32_t* extensionCount) {return glfwGetRequiredInstanceExtensions(extensionCount); }


        inline GLFWwindow* GetNativeWindow() const { return m_window; }
        virtual void SetEventCallback(const EventCallbackFn& callback) { m_data.m_eventCallbackFn = callback; };

#ifdef LG_VULKAN_API
        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
#endif // LG_VULKAN_API

    private:
        static void FrameBufferResizedCB(GLFWwindow* window, int width, int height);

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
