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
        virtual void Init() override;
        virtual bool Update() override;

        inline bool WasWindowResized() const { return m_frameBufferResized; };
        inline void ResetWindowResizedFlag() { m_frameBufferResized = false; };
        //Vulkan Specific
        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        VkExtent2D GetExtent();

    private:
        static void FrameBufferResizedCB(GLFWwindow* window, int width, int height);

    private:
        GLFWwindow* m_window = nullptr;
        bool m_frameBufferResized = false;
    };
} //namespace LG
