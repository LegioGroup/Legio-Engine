#include <Legio/editor/Editor.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <Legio/LegioCore.h>
#include "platform/EngineWindow.h"
#include "rendering/vulkan/VKRenderer.h"

namespace LG
{
    static void check_vk_result(VkResult err)
    {
        if (err == 0)
            return;
        //fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        LG_CORE_ERROR( "Vulkan Error: {0}", err);
        if (err < 0)
            abort();
    }
    // Shapes
    constexpr float k_roundness = 2.0f;
    // Font
    constexpr float k_font_size  = 24.0f;
    constexpr float k_font_scale = 0.7f;
    // Color
    constexpr ImVec4 k_color_text                = ImVec4(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
    constexpr ImVec4 k_color_text_disabled       = ImVec4(54.0f / 255.0f, 54.0f / 255.0f, 54.0f / 255.0f, 1.0f);
    constexpr ImVec4 k_color_dark_very           = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 1.0f);
    constexpr ImVec4 k_color_dark                = ImVec4(21.0f  / 255.0f, 21.0f  / 255.0f, 21.0f  / 255.0f, 1.0f);
    constexpr ImVec4 k_color_mid                 = ImVec4(36.0f  / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 1.0f);
    constexpr ImVec4 k_color_light               = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 1.0f);
    constexpr ImVec4 k_color_shadow              = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
    constexpr ImVec4 k_color_interactive         = ImVec4(56.0f / 255.0f, 56.0f / 255.0f, 56.0f / 255.0f, 1.0f);
    constexpr ImVec4 k_color_interactive_hovered = ImVec4(0.450f, 0.450f, 0.450f, 1.000f);
    constexpr ImVec4 k_color_check               = ImVec4(26.0f / 255.0f, 140.0f / 255.0f, 192.0f / 255.0f, 1.0f);

    static void ImGui_Initialise()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        //Config
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
        io.ConfigWindowsResizeFromEdges = true;
        io.ConfigViewportsNoTaskBarIcon = true;

        // io.Fonts->AddFontFromFileTTF("../../external/engine/data/fonts/Calibri.ttf", k_font_size);
        // io.FontGlobalScale = k_font_scale;
        ImGui_ImplGlfw_InitForVulkan(static_cast<EngineWindow*>(ServiceLocator::GetWindow())->GetNativeWindow(), true);

        VKRenderer* renderer = static_cast<VKRenderer*>(ServiceLocator::GetRenderer());
        const VKRenderer::Vulkan_Editor_InitInfo vkInitinfo = renderer->GetRendererEditorInitInfo();
        ImGui_ImplVulkan_InitInfo vkInfo = {};

        vkInfo.Instance = vkInitinfo.Instance;
        vkInfo.PhysicalDevice = vkInitinfo.PhysicalDevice;
        vkInfo.Device = vkInitinfo.Device;
        vkInfo.QueueFamily = vkInitinfo.QueueFamily;
        vkInfo.Queue = vkInitinfo.Queue;
        vkInfo.PipelineCache = vkInitinfo.PipelineCache;
        vkInfo.DescriptorPool = vkInitinfo.DescriptorPool;
        vkInfo.Subpass = vkInitinfo.Subpass;
        vkInfo.MinImageCount = vkInitinfo.MinImageCount;
        vkInfo.ImageCount = vkInitinfo.ImageCount;
        vkInfo.MSAASamples = vkInitinfo.MSAASamples;
        vkInfo.Allocator = vkInitinfo.Allocator;
        vkInfo.CheckVkResultFn = check_vk_result;

        VkAttachmentDescription attachment = {};
        attachment.format = renderer->GetSwapChain()->GetSwapChainImageFormat();
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment = {};
        color_attachment.attachment = 0;
        color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPass imGuiRenderPass;
        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;
        info.dependencyCount = 1;
        info.pDependencies = &dependency;
        if (vkCreateRenderPass(vkInfo.Device, &info, nullptr, &imGuiRenderPass) != VK_SUCCESS) {
            throw std::runtime_error("Could not create Dear ImGui's render pass");
        }


        ImGui_ImplVulkan_Init(&vkInfo, imGuiRenderPass);
        
        VkCommandBuffer command_buffer = beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
        endSingleTimeCommands(command_buffer);   
    }

    static void ImGui_ShutDown()
    {
        if(ImGui::GetCurrentContext())
        {
            ImGui::DestroyContext();
        }
    }

    static void ImGui_ApplyColors()
    {
        // Use default dark style as a base
        ImGui::StyleColorsDark();
        ImVec4* colors = ImGui::GetStyle().Colors;

        // Colors
        colors[ImGuiCol_Text]                  = k_color_text;
        colors[ImGuiCol_TextDisabled]          = k_color_text_disabled;
        colors[ImGuiCol_WindowBg]              = k_color_mid;                   // Background of normal windows
        colors[ImGuiCol_ChildBg]               = k_color_mid;                   // Background of child windows
        colors[ImGuiCol_PopupBg]               = k_color_dark;                  // Background of popups, menus, tooltips windows
        colors[ImGuiCol_Border]                = k_color_interactive;
        colors[ImGuiCol_BorderShadow]          = k_color_shadow;
        colors[ImGuiCol_FrameBg]               = k_color_dark_very;             // Background of checkbox, radio button, plot, slider, text input
        colors[ImGuiCol_FrameBgHovered]        = k_color_interactive;
        colors[ImGuiCol_FrameBgActive]         = k_color_dark_very;
        colors[ImGuiCol_TitleBg]               = k_color_dark;
        colors[ImGuiCol_TitleBgActive]         = k_color_dark;
        colors[ImGuiCol_TitleBgCollapsed]      = k_color_light;
        colors[ImGuiCol_MenuBarBg]             = k_color_dark;
        colors[ImGuiCol_ScrollbarBg]           = k_color_mid;
        colors[ImGuiCol_ScrollbarGrab]         = k_color_interactive;
        colors[ImGuiCol_ScrollbarGrabHovered]  = k_color_interactive_hovered;
        colors[ImGuiCol_ScrollbarGrabActive]   = k_color_dark_very;
        colors[ImGuiCol_CheckMark]             = k_color_check;
        colors[ImGuiCol_SliderGrab]            = k_color_interactive;
        colors[ImGuiCol_SliderGrabActive]      = k_color_dark_very;
        colors[ImGuiCol_Button]                = k_color_interactive;
        colors[ImGuiCol_ButtonHovered]         = k_color_interactive_hovered;
        colors[ImGuiCol_ButtonActive]          = k_color_dark_very;
        colors[ImGuiCol_Header]                = k_color_light;                 // Header colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
        colors[ImGuiCol_HeaderHovered]         = k_color_interactive_hovered;
        colors[ImGuiCol_HeaderActive]          = k_color_dark_very;
        colors[ImGuiCol_Separator]             = k_color_dark_very;
        colors[ImGuiCol_SeparatorHovered]      = k_color_light;
        colors[ImGuiCol_SeparatorActive]       = k_color_light;
        colors[ImGuiCol_ResizeGrip]            = k_color_interactive;
        colors[ImGuiCol_ResizeGripHovered]     = k_color_interactive_hovered;
        colors[ImGuiCol_ResizeGripActive]      = k_color_dark_very;
        colors[ImGuiCol_Tab]                   = k_color_light;
        colors[ImGuiCol_TabHovered]            = k_color_interactive_hovered;
        colors[ImGuiCol_TabActive]             = k_color_dark_very;
        colors[ImGuiCol_TabUnfocused]          = k_color_light;
        colors[ImGuiCol_TabUnfocusedActive]    = k_color_light;                 // Might be called active, but it's active only because it's it's the only tab available, the user didn't really activate it
        colors[ImGuiCol_DockingPreview]        = k_color_dark_very;             // Preview overlay color when about to docking something
        colors[ImGuiCol_DockingEmptyBg]        = k_color_interactive;           // Background color for empty node (e.g. CentralNode with no window docked into it)
        colors[ImGuiCol_PlotLines]             = k_color_interactive;
        colors[ImGuiCol_PlotLinesHovered]      = k_color_interactive_hovered;
        colors[ImGuiCol_PlotHistogram]         = k_color_interactive;
        colors[ImGuiCol_PlotHistogramHovered]  = k_color_interactive_hovered;
        colors[ImGuiCol_TextSelectedBg]        = k_color_dark;
        colors[ImGuiCol_DragDropTarget]        = k_color_interactive_hovered;   // Color when hovering over target
        colors[ImGuiCol_NavHighlight]          = k_color_dark;                  // Gamepad/keyboard: current highlighted item
        colors[ImGuiCol_NavWindowingHighlight] = k_color_dark;                  // Highlight window when using CTRL+TAB
        colors[ImGuiCol_NavWindowingDimBg]     = k_color_dark;                  // Darken/colorize entire screen behind the CTRL+TAB window list, when active
        colors[ImGuiCol_ModalWindowDimBg]      = k_color_dark;                  // Darken/colorize entire screen behind a modal window, when one is active
    }

    static void ImGui_ApplyStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowBorderSize         = 1.0f;
        style.FrameBorderSize          = 1.0f;
        style.ScrollbarSize            = 20.0f;
        style.FramePadding             = ImVec2(5, 5);
        style.ItemSpacing              = ImVec2(6, 5);
        style.WindowMenuButtonPosition = ImGuiDir_Right;
        style.WindowRounding           = k_roundness;
        style.FrameRounding            = k_roundness;
        style.PopupRounding            = k_roundness;
        style.GrabRounding             = k_roundness;
        style.ScrollbarRounding        = k_roundness;
        style.Alpha                    = 1.0f;
    }

    void Editor::BeginWindow()
    {
        const auto windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | 
                                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
                                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                 ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Set window style
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowBgAlpha(0.0f);

        // Begin window
        std::string name = "##main_window";
        bool open = true;
        m_editorStarted = ImGui::Begin(name.c_str(), &open, windowFlags);
        ImGui::PopStyleVar(3);

        //Docking Space
        if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable && m_editorStarted)
        {
            //Dock space
            const auto windowID = ImGui::GetID(name.c_str());
            if(!ImGui::DockBuilderGetNode(windowID))
            {
                ImGui::DockBuilderRemoveNode(windowID);
                ImGui::DockBuilderAddNode(windowID, ImGuiDockNodeFlags_None);
                ImGui::DockBuilderSetNodeSize(windowID, ImGui::GetMainViewport()->Size);

                ImGuiID dock_main_id = windowID;
                ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id,  ImGuiDir_Right, 0.2f,  nullptr, &dock_main_id);
                const ImGuiID dock_right_down_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Down,  0.6f,  nullptr, &dock_right_id);
                ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id,  ImGuiDir_Down,  0.25f, nullptr, &dock_main_id);
                const ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id,  ImGuiDir_Right, 0.6f,  nullptr, &dock_down_id);

                            // Dock windows
                ImGui::DockBuilderDockWindow("World",      dock_right_id);
                ImGui::DockBuilderDockWindow("Properties", dock_right_down_id);
                ImGui::DockBuilderDockWindow("Console",    dock_down_id);
                ImGui::DockBuilderDockWindow("Assets",     dock_down_right_id);
                ImGui::DockBuilderDockWindow("Viewport",   dock_main_id);

                ImGui::DockBuilderFinish(dock_main_id);
            }
        
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            ImGui::DockSpace(windowID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
            ImGui::PopStyleVar();
        }
    }

    Editor::~Editor()
    {
        ImGui_ShutDown();
    }

    void Editor::Init()
    {
        ImGui_Initialise();
        ImGui_ApplyColors();
        ImGui_ApplyStyle();
    }

    void Editor::Update()
    {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplVulkan_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        BeginWindow();

        //Update Widgets

        if(m_editorStarted)
        {
            ImGui::End();
        }

        ImGui::Render();
        VKRenderer* renderer = static_cast<VKRenderer*>(ServiceLocator::GetRenderer());
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),renderer->GetCurrentCommandBuffer());
        //Render Stuff
    }

    
} // namespace LG
