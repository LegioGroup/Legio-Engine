#pragma once

namespace LG
{
    class Window
    {
    public:
        virtual void OpenWindow() = 0;
        virtual bool Update() = 0;
    };
} //namespace LG
