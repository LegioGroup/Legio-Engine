#pragma once

#include "rendering/vulkan/VKDevice.h"
#include "rendering/vulkan/VKPipeline.h"
#include <memory>
#include <functional>

namespace LG
{
    class VKSwapChain
    {
    public:
        VKSwapChain(VKDevice* device, VkExtent2D extent);
        VKSwapChain(VKDevice* device, VkExtent2D extent, std::shared_ptr<VKSwapChain> previous);
        ~VKSwapChain();

        VKSwapChain(const VKSwapChain&) = delete;
        void operator=(const VKSwapChain&) = delete;

        VkFramebuffer GetFrameBuffer(int index) { return m_swapChainFrameBuffers[index]; }
        VkRenderPass GetRenderPass() { return m_renderPass; }
        VkImageView GetImageView(int index) { return m_swapChainImageViews[index]; }
        size_t ImageCount() { return m_swapChainImages.size(); }
        VkFormat GetSwapChainImageFormat() { return m_swapChainImageFormat; }
        VkExtent2D GetSwapChainExtent() { return m_swapChainExtent; }

        uint32_t Width() { return m_swapChainExtent.width; }
        uint32_t Height() { return m_swapChainExtent.height; }

        float ExtentAspectRatio() {
            return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height);
        }

        VkResult AcquireNextImage(uint32_t* imageIndex);
        void ResetFences(const VkCommandBuffer* buffers);
        void SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);
    private:
        void Init();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateFrameBuffers();
        void CreateSyncObjects();

        void CleanupSwapChain();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
    private:
        VKDevice* m_device;

        std::shared_ptr<VKSwapChain> m_oldSwapChain;
        VkSwapchainKHR m_swapChain;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        std::vector<VkFramebuffer> m_swapChainFrameBuffers;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;
        VkRenderPass m_renderPass;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        uint32_t m_currentFrame = 0;
    };
} // namespace LG
