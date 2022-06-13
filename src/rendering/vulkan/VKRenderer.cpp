#include <array>
#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>
#include "platform/EngineWindow.h"
#include "platform/WindowsInput.h"

#include "rendering/vulkan/VKRenderer.h"

namespace LG
{
    // local callback functions
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) 
    {
        switch (messageSeverity)
        {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LG_CORE_ERROR(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LG_CORE_WARN(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            LG_CORE_INFO(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            LG_CORE_TRACE(pCallbackData->pMessage);
            break;
        }
        return VK_FALSE;
    }

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    bool VKRenderer::CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    VKRenderer::~VKRenderer()
    {
        LG_CORE_INFO("Shutting Down Vulkan Renderer!");
    }

    void VKRenderer::Init(RendererSettings settings)
    {
        LG_CORE_INFO("Initialize Vulkan Renderer!"); 
        CreateInstance();
    }

    void VKRenderer::CreateInstance()
    {
        if (enableValidationLayers && !CheckValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Legio Renderer";
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "Legio Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        uint32_t extensionCount = 0;
        const char** extensions;
        
        extensions = EngineWindow::GetRequiredInstanceExtension(&extensionCount);

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = 0;
        createInfo.enabledExtensionCount = extensionCount;
        createInfo.ppEnabledExtensionNames = extensions;

        if(enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            LG_CORE_CRITICAL("failed to create instance!");
            throw std::runtime_error("failed to create instance!");
        }
    }

    void VKRenderer::Shutdown()
    {
    }

    void VKRenderer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<FrameBufferResizeEvent>(LG_BIND_EVENT_FN(VKRenderer::OnFrameBufferResizeEvent));
        dispatcher.Dispatch<AppTickEvent>(LG_BIND_EVENT_FN(VKRenderer::OnAppTickEvent));
    }

    bool VKRenderer::OnAppTickEvent(AppTickEvent& event)
    {
        float fixedTick = event.GetFixedTick();

        return false;
    }

    bool VKRenderer::OnFrameBufferResizeEvent(FrameBufferResizeEvent& e)
    {
        return false;
    }

    void VKRenderer::RenderFrame()
    {
        
    }

    void VKRenderer::RendererWaitIdle()
    {
    }
} // namespace LG
