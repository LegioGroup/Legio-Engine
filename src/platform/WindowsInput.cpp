#include <Legio/ServiceLocator.h>
#include "platform/WindowsInput.h"
#include "platform/EngineWindow.h"

namespace LG
{
    bool WindowsInput::IsKeyPressed(int keyCode)
    {
        const auto& window = static_cast<EngineWindow*>(ServiceLocator::GetWindow())->GetNativeWindow();
        int state = glfwGetKey(window, keyCode);
        return state == (GLFW_PRESS || GLFW_REPEAT);
    }

    bool WindowsInput::IsMouseButtonPressed(int button)
    {
        const auto& window = static_cast<EngineWindow*>(ServiceLocator::GetWindow())->GetNativeWindow();
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    glm::vec2 WindowsInput::GetMousePosition()
    {
        const auto& window = static_cast<EngineWindow*>(ServiceLocator::GetWindow())->GetNativeWindow();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return { (float)xPos, (float)yPos };
    }

    float WindowsInput::GetMouseX()
    {
        auto mousePos = GetMousePosition();
        return mousePos.x;
    }

    float WindowsInput::GetMouseY()
    {
        auto mousePos = GetMousePosition();
        return mousePos.y;
    }
} // namespace LG
