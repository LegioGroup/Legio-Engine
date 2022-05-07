#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include "rendering/vulkan/VKSwapChain.h"
#include "rendering/vulkan/VKCamera.h"
#include "rendering/vulkan/renderSystems/VKSimpleRenderSystem.h"
namespace LG
{
    class VKRenderer : public Renderer
    {
    public:
        VKRenderer() = default;
        VKRenderer(const VKRenderer&) = delete;
        VKRenderer& operator=(const VKRenderer&) = delete;

        virtual ~VKRenderer();

        virtual void Init(RendererSettings settings) override;
        virtual void Shutdown() override;
        virtual void RenderFrame()  override;
        virtual void RendererWaitIdle() override;
        inline float GetAspectRatio() const { return m_swapChain->ExtentAspectRatio(); }
    private:

        void BeginFrame();
        void EndFrame();

        void BeginSwapChainRenderPass();
        void EndSwapChainRenderPass();

        void CreateCommandBuffers();
        void FreeCommandBuffers();

        void RecreateSwapChain();
    private:
        EngineWindow* m_engineWindow;
        std::unique_ptr<VKDevice> m_device;
        std::unique_ptr<VKSwapChain> m_swapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::unique_ptr<VKSimpleRenderSystem> m_simpleRenderSystem;
        VKCamera m_camera;
        uint32_t m_currentImageIndex;
        bool m_isFrameStarted;
    };
} // namespace LG
