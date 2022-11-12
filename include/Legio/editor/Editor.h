#pragma once

namespace LG 
{
    class Editor 
    {
    public:
        Editor() = default;
        void Init();
        void Shutdown();

        void BeginFrame();
        void RenderFrame();

    };
}