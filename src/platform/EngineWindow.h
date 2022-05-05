#pragma once
#include <Legio/platform/Window.h>
#include <GLFW/glfw3.h>
namespace LG
{
    class EngineWindow : public Window
    {
    public:
        EngineWindow() = default;
        ~EngineWindow();
        virtual void OpenWindow(WindowSettings data = WindowSettings()) override;
        virtual bool Update() override;
    private:
        GLFWwindow* m_window = nullptr;
    };
} //namespace LG
