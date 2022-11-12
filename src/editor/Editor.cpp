#include <Legio/ServiceLocator.h>
#include <platform/EngineWindow.h>
#include <Legio/editor/Editor.h>
#include <imgui.h>
#include <Legio/editor/imgui/imgui_impl_glfw.h>
#include <Legio/editor/imgui/imgui_impl_opengl3.h>
namespace LG 
{
    void Editor::Init()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(static_cast<EngineWindow*>(ServiceLocator::GetWindow())->GetNativeWindow(), false);
        ImGui_ImplOpenGL3_Init();

    }

    void Editor::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Editor::BeginFrame()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Editor::RenderFrame()
    {
        static bool someBoolean = false;
        static float speed = 0.0f;
        ImGui::Begin("MyWindow");
        ImGui::Checkbox("Boolean property", &someBoolean);
        if (ImGui::Button("Reset Speed")) {
            speed = 0;
        }
        ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
        
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}