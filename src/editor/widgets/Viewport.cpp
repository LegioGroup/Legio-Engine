#include "Legio/editor/widgets/Viewport.h"
#include <Legio/ServiceLocator.h>
#include <imgui.h>
namespace LG 
{
    void Viewport::Render()
    {
        ImGui::Begin("Viewport");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("GameRender");
            // Get the size of the child (i.e. the whole draw size of the windows).
            float width = ImGui::GetContentRegionAvail().x;
            float height = ImGui::GetContentRegionAvail().y;
            // Because I use the texture from OpenGL, I need to invert the V from the UV.
            ImGui::Image((ImTextureID)ServiceLocator::GetRenderer()->GetRenderTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();
    }
}

