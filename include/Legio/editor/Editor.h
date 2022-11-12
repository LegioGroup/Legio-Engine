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
        void OnEvent(Event& event);
    private:
        bool OnAppTickEvent(AppTickEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);

        void ApplyColors();
        void ApplyStyle();
    };
}