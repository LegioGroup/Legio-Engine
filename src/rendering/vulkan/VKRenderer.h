#pragma once
#include <vulkan/vulkan.h>
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <optional>
#include <memory.h>
#include "rendering/vulkan/VKPipeline.h"
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
        //------------------Command Pool-----------------------
        void CreateCommandPool();
        //------------------Command Buffer---------------------
        void CreateCommandBuffers();
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        //------------------Surface----------------------------
        void CreateSurface();
        //------------------Swap Chains------------------------
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void CreateSwapChain();
        void CreateFrameBuffers();
        void CreateSyncObjects();
        //ImageViews
        void CreateImageViews();
        //--------------------Graphics Pipeline-----------------------
        void CreateGraphicsPipeline();
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
        uint32_t m_currentFrame = 0;
        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device;
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        VkSurfaceKHR m_surface;
        VkSwapchainKHR m_swapChain;
        std::vector<VkFramebuffer> m_swapChainFrameBuffers;
        std::unique_ptr<VKPipeline> m_pipeline;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        VkDebugUtilsMessengerEXT m_debugMessenger;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
    };
} // namespace LG
