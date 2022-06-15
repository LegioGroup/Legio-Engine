#pragma once
#include <vulkan/vulkan.h>
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <optional>
#include <memory.h>
#include "rendering/vulkan/VkDevice.h"
#include "rendering/vulkan/VKPipeline.h"
namespace LG
{
    class VKRenderer : public Renderer
    {
    public:
        VKRenderer() = default;
        VKRenderer(const VKRenderer&) = delete;
        VKRenderer& operator=(const VKRenderer&) = delete;

        virtual void Init(RendererSettings settings) override;
        virtual void Shutdown() override;
        virtual void RenderFrame() override;
        virtual void RendererWaitIdle() override;
        virtual void OnEvent(Event& event) override;
    private:
        void CreateCommandBuffers();
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void CreateSwapChain();
        void CreateFrameBuffers();
        void CreateSyncObjects();
        void RecreateSwapChain();
        void CleanupSwapChain();
        //ImageViews
        void CreateImageViews();
        void CreateGraphicsPipeline();
        bool OnFrameBufferResizeEvent(FrameBufferResizeEvent& e);
        bool OnAppTickEvent(AppTickEvent& event);
    private:
        uint32_t m_currentFrame = 0;
        std::unique_ptr<VKDevice> m_device;
        
        std::vector<VkCommandBuffer> m_commandBuffers;
        VkSwapchainKHR m_swapChain;
        std::vector<VkFramebuffer> m_swapChainFrameBuffers;
        std::unique_ptr<VKPipeline> m_pipeline;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        bool m_frameBufferResized = false;

    };
} // namespace LG
