#pragma once
#include <Legio/application/Events/Event.h>
namespace LG
{
    struct WindowProps
    {
        WindowProps(int width = 800, int height = 600, const char* name = "MainWindow")
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
        using EventCallbackFn = std::function<void(Event&)>;

        Window(WindowProps windowSettings = WindowProps()) {};
        virtual void Init() = 0;
        virtual bool Update() = 0;
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;
        virtual void SwapWindowBuffers() = 0;

    };
} //namespace LG
