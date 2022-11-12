#include "Legio/editor/widgets/Viewport.h"
#include <Legio/ServiceLocator.h>
#include <imgui.h>
namespace LG 
{
    void Viewport::Render()
    {
        ImGui::BeginChild("Viewport");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("GameRender");
            // Get the size of the child (i.e. the whole draw size of the windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V from the UV.
            ImGui::Image((ImTextureID)ServiceLocator::GetRenderer()->GetRenderTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
}

