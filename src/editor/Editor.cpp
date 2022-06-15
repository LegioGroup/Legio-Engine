#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <Legio/editor/Editor.h>
#include <Legio/ServiceLocator.h>
#include "platform/EngineWindow.h"
#include "rendering/vulkan/VKRenderer.h"

namespace LG
{
    static void check_vk_result(VkResult err)
    {
        if (err == 0)
            return;
        //fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        LG_CORE_CRITICAL( "Vulkan CRITICAL: {0}", err);
        if (err < 0)
            abort();
    }

    void Editor::Init()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        auto* window = static_cast<EngineWindow*>(ServiceLocator::GetWindow());
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForVulkan(window->GetNativeWindow(), true);
        LG::VKImGui_ImplVulkan_InitInfo info = static_cast<VKRenderer*>(ServiceLocator::GetRenderer())->GetImguiVulkanInitInfo();

        VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        VkResult err = vkCreateDescriptorPool(info.Device, &pool_info, nullptr, &descriptorPool);
        check_vk_result(err);


        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = info.Instance;
        init_info.PhysicalDevice = info.PhysicalDevice;
        init_info.Device = info.Device;
        init_info.QueueFamily = info.QueueFamily;
        init_info.Queue = info.Queue;
        init_info.PipelineCache =info.PipelineCache;
        init_info.DescriptorPool = descriptorPool;
        init_info.Allocator = info.Allocator;
        init_info.MinImageCount = info.MinImageCount;
        init_info.ImageCount = info.ImageCount;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info,  static_cast<VKRenderer*>(ServiceLocator::GetRenderer())->GetImguiRenderPass());
        
    }

    void Editor::Update()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
    }
    
    void Editor::BeginWindow(){}
} // namespace LG
