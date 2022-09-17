#pragma once
#include <vulkan/vulkan.h>
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include <Legio/rendering/LGCamera.h>
#include <optional>
#include <memory.h>
#include "rendering/vulkan/Vulkan_Device.h"
#include "rendering/vulkan/Vulkan_GameObject.h"
#include "rendering/vulkan/Vulkan_SwapChain.h"
#include "rendering/vulkan/Vulkan_Model.h"
#include "rendering/vulkan/renderSystems/Vulkan_GeometryRenderSystem.h"
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
        void RecreateSwapChain();
        void FreeCommandBuffers();
        void CreateCommandBuffers();
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        bool OnFrameBufferResizeEvent(FrameBufferResizeEvent& e);
        bool OnAppTickEvent(AppTickEvent& event);
    private:

        std::unique_ptr<VKDevice> m_device;
        std::unique_ptr<VKSwapChain> m_swapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::unique_ptr<GeometryBasicRenderSystem> m_renderSystem;
        LGCamera m_camera = {};
        VKGameObject m_cameraGameObject = VKGameObject::CreateGameObject();
        uint32_t m_currentImageIndex = 0;
        bool m_needRecreateSwapChain = false;

    };
} // namespace LG
