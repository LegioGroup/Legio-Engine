#pragma once

namespace LG
{
    class Window
    {
    public:
        virtual ~Window() = default;
        virtual void OpenWindow() = 0;
        virtual bool Update() = 0;
    };
} //namespace LG
