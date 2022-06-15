#pragma once
#include <vulkan/vulkan.h>
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <optional>
#include <memory.h>
#include "rendering/vulkan/VKPipeline.h"
namespace LG
{
    struct VKImGui_ImplVulkan_InitInfo
    {
        VkInstance                      Instance;
        VkPhysicalDevice                PhysicalDevice;
        VkDevice                        Device;
        uint32_t                        QueueFamily;
        VkQueue                         Queue;
        VkPipelineCache                 PipelineCache;
        VkDescriptorPool                DescriptorPool;
        uint32_t                        Subpass;
        uint32_t                        MinImageCount;          // >= 2
        uint32_t                        ImageCount;             // >= MinImageCount
        VkSampleCountFlagBits           MSAASamples;            // >= VK_SAMPLE_COUNT_1_BIT (0 -> default to VK_SAMPLE_COUNT_1_BIT)
        const VkAllocationCallbacks*    Allocator;
        void                            (*CheckVkResultFn)(VkResult err);
    };

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
        const VKImGui_ImplVulkan_InitInfo& GetImguiVulkanInitInfo() const { return m_imguiInitInfo; }
        VkRenderPass GetImguiRenderPass() const { return m_pipeline->GetImguiRenderPass(); }
    
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
        void CreateCommandPool(VkCommandPool* commandPool);
        //------------------Command Buffer---------------------
        void CreateCommandBuffer();
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        VkCommandBuffer BeginSingleTimeCommands(const VkCommandPool &cmdPool);
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer, const VkCommandPool &cmdPool);
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
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        //Imgui
        void CreateImguiFrameBuffers();
        void CreateImguiCommandBuffers();
        // Viewport
        void CreateViewportImage();
        void CreateViewportImageViews();
        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
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
        //----------------------Utilities----------------------------------
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void InsertImageMemoryBarrier(VkCommandBuffer cmdbuffer,
                                          VkImage image,
                                          VkAccessFlags srcAccessMask,
                                          VkAccessFlags dstAccessMask,
                                          VkImageLayout oldImageLayout,
                                          VkImageLayout newImageLayout,
                                          VkPipelineStageFlags srcStageMask,
                                          VkPipelineStageFlags dstStageMask,
                                          VkImageSubresourceRange subresourceRange)

    private:
        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device;
        VkCommandPool m_commandPool;
        VkCommandPool m_imguiCommandPool;
        VkCommandBuffer m_commandBuffer;
        VkCommandBuffer m_ImGuiCommandBuffer;
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        VkSurfaceKHR m_surface;
        VkSwapchainKHR m_swapChain;
        std::vector<VkFramebuffer> m_swapChainFrameBuffers;
        std::vector<VkFramebuffer> m_imguiFrameBuffers;
        std::unique_ptr<VKPipeline> m_pipeline;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        VkDebugUtilsMessengerEXT m_debugMessenger;

        // viewport
        std::vector<VkImage> m_viewportImages;
        std::vector<VkDeviceMemory> m_DstImageMemory;
        std::vector<VkImageView> m_viewportImageViews;
        VkRenderPass m_viewportRenderPass;
        VkPipeline m_viewportPipeline;
        VkCommandPool m_viewportCommandPool;
        std::vector<VkFramebuffer> m_viewportFramebuffers;
        std::vector<VkCommandBuffer> m_viewportCommandBuffers;
        std::vector<VkDescriptorSet> m_Dset;
        VkSampler m_TextureSampler;

        VkSemaphore m_imageAvailableSemaphore;
        VkSemaphore m_renderFinishedSemaphore;
        VkFence m_inFlightFence;

        VKImGui_ImplVulkan_InitInfo m_imguiInitInfo;
    };
} // namespace LG
