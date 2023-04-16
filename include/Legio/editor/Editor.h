#pragma once
#include  <functional>
#include <Legio/editor/widgets/IWidget.h>
namespace LG 
{
    class Editor 
    {
    public:
        Editor() = default;
        ~Editor();
        void Init();
        void Shutdown();

        void BeginFrame();
        void RenderFrame();
        void OnEvent(Event& event);
    private:
        bool OnAppTickEvent(AppTickEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);
        bool OnWindowResizedEvent(WindowResizeEvent& event);


        void ApplyColors();
        void ApplyStyle();
        void RenderEditorDockSpace();
    private:
        std::vector<std::unique_ptr<IWidget>> m_widgets;
        std::function<void()> m_createChildWidgets_Fn;
    };
}