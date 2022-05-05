#pragma once

#include "rendering/vulkan/VKdevice.h"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>

namespace LG {

    class VKSwapChain {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        VKSwapChain(VKDevice& deviceRef, VkExtent2D windowExtent);
        ~VKSwapChain();

        VKSwapChain(const VKSwapChain&) = delete;
        void operator=(const VKSwapChain&) = delete;

        VkFramebuffer GetFrameBuffer(int index) { return m_swapChainFramebuffers[index]; }
        VkRenderPass GetRenderPass() { return m_renderPass; }
        VkImageView GetImageView(int index) { return m_swapChainImageViews[index]; }
        size_t ImageCount() { return m_swapChainImages.size(); }
        VkFormat GetSwapChainImageFormat() { return m_swapChainImageFormat; }
        VkExtent2D GetSwapChainExtent() { return swapChainExtent; }
        uint32_t Width() { return swapChainExtent.width; }
        uint32_t Height() { return swapChainExtent.height; }

        float ExtentAspectRatio() {
            return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
        }
        VkFormat FindDepthFormat();

        VkResult AcquireNextImage(uint32_t* imageIndex);
        VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

    private:
        void CreateSwapChain();
        void CreateImageViews();
        void CreateDepthResources();
        void createRenderPass();
        void CreateFramebuffers();
        void CreateSyncObjects();

        // Helper functions
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkFormat m_swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkFramebuffer> m_swapChainFramebuffers;
        VkRenderPass m_renderPass;

        std::vector<VkImage> m_depthImages;
        std::vector<VkDeviceMemory> m_depthImageMemorys;
        std::vector<VkImageView> m_depthImageViews;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;

        VKDevice& m_device;
        VkExtent2D m_windowExtent;

        VkSwapchainKHR m_swapChain;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        std::vector<VkFence> m_imagesInFlight;
        size_t m_currentFrame = 0;
    };

}  // namespace lve