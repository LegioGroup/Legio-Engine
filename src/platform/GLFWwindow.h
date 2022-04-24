#pragma once
#include <Legio/platform/Window.h>
#include <GLFW/glfw3.h>
namespace LG
{
    class CustomWindow : public Window
    {
    public:
        CustomWindow() = default;
        virtual void OpenWindow() override;
        virtual bool Update() override;
    private:
        GLFWwindow* m_window = nullptr;
    };
} //namespace LG
