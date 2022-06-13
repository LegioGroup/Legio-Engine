#pragma once
#include <vulkan/vulkan.h>
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <optional>
namespace LG
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;

        bool IsComplete()
        {
            return graphicsFamily.has_value();
        }
    };
    
    class VKRenderer : public Renderer
    {
    public:
        VKRenderer() = default;
        VKRenderer(const VKRenderer&) = delete;
        VKRenderer& operator=(const VKRenderer&) = delete;

        virtual ~VKRenderer();

        virtual void Init(RendererSettings settings) override;
        virtual void Shutdown() override;
        virtual void RenderFrame() override;
        virtual void RendererWaitIdle() override;
        virtual void OnEvent(Event& event) override;
    private:
        //------------------Instance-------------------------
        void CreateInstance();
        //------------------Physical Devices-------------------
        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice device);
        //------------------Queue Families---------------------
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        //------------------Logical Device---------------------
        void CreateLogicalDevice();
        //-----------------------------------------------------
        bool OnFrameBufferResizeEvent(FrameBufferResizeEvent& e);
        bool OnAppTickEvent(AppTickEvent& event);
        //---------------------Validation Layer Support------------------
        bool CheckValidationLayerSupport();
        std::vector<const char*> GetRequiredExtensions();
        void SetupDebugMessenger();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        //-----------------------------------------------------------------
    private:
        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device;
        VkQueue m_graphicsQueue;
        VkDebugUtilsMessengerEXT m_debugMessenger;
    };
} // namespace LG
