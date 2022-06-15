#pragma once 
#include <optional>
#include <vulkan/vulkan.h>
#include <vector>
namespace LG
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool IsComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
    
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VKDevice
    {
    public:
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

        VKDevice();
        ~VKDevice();

        VKDevice(const VKDevice&) = delete;
        void operator=(const VKDevice&) = delete;
        VKDevice(VKDevice&&) = delete;
        VKDevice& operator=(VKDevice&&) = delete;

        inline VkCommandPool GetCommandPool() const { return m_commandPool; }
        inline VkDevice GetDevice() const { return m_device; }
        inline VkSurfaceKHR GetSurface() const { return m_surface; }
        inline VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
        inline VkQueue GetPresentQueue() const { return m_presentQueue; }

        inline SwapChainSupportDetails GetSwapChainSupportDetails() const { return QuerySwapChainSupport(m_physicalDevice); }
        inline QueueFamilyIndices FindPhysicalQueueFamilies() const { return FindQueueFamilies(m_physicalDevice); }


    private:
        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateCommandPool();

        //-----------------Validation Layers--------------------
        bool CheckValidationLayerSupport();
        std::vector<const char*> GetRequiredExtensions();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        //-----------------Helper Functions--------------------
        bool IsDeviceSuitable(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;


    private:
        VkInstance m_instance;
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;
        VkSurfaceKHR m_surface;
        VkCommandPool m_commandPool;

        const std::vector<const char*> m_validationLayers = { "VK_LAYER_KHRONOS_validation" };
        const std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    };    
} // namespace LG

