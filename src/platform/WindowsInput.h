#pragma once
#include <Legio/platform/Input.h>

namespace LG
{
    class WindowsInput : public Input
    {
    public:
        WindowsInput() = default;
        virtual ~WindowsInput() = default;

        virtual void Init() override {}
        virtual bool IsKeyPressed(int keyCode) override;
        virtual bool IsMouseButtonPressed(int button) override;
        virtual glm::vec2 GetMousePosition() override;
        virtual float GetMouseX() override;
        virtual float GetMouseY() override;
    };
} // namespace LG
