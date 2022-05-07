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
        Window(WindowSettings windowSettings = WindowSettings()) : m_windowSettings(windowSettings) {};
        virtual ~Window() = default;
        virtual void Init() = 0;
        virtual bool Update() = 0;
        inline WindowSettings GetWindowSettings() const { return m_windowSettings; };
        inline void SetWindowSettings(const WindowSettings& newWindowSettings){ m_windowSettings = newWindowSettings; };
    protected:
        WindowSettings m_windowSettings;
    };
} //namespace LG
