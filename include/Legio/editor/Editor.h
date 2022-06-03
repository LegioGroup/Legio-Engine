#pragma once

namespace LG
{
    class Editor
    {
    public:
        Editor() = default;
        ~Editor();
        void Init();
        void Update();
        void BeginWindow();
    private:
        bool m_editorStarted;
    };
} // namespace LG
