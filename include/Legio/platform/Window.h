#pragma once

namespace LG
{
    struct WindowSettings
    {
        WindowSettings(int width = 800, int height = 600, const char* name = "MainWindow")
            : m_width(width)
            , m_height(height)
            , m_name(name)
        {}

        const char* m_name;
        int m_width;
        int m_height;
    };

    class Window
    {
    public:
        virtual ~Window() = default;
        virtual void OpenWindow(WindowSettings data = WindowSettings()) = 0;
        virtual bool Update() = 0;
    };
} //namespace LG
