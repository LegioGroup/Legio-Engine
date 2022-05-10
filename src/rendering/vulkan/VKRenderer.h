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
        virtual void OnEvent(Event& event) override;
    private:

        void BeginFrame();
        void EndFrame();

        void BeginSwapChainRenderPass();
        void EndSwapChainRenderPass();

        void CreateCommandBuffers();
        void FreeCommandBuffers();

        void RecreateSwapChain();

        bool OnFrameBufferResizeEvent(FrameBufferResizeEvent& e);
        bool OnAppTickEvent(AppTickEvent& event);
    private:
        EngineWindow* m_engineWindow;
        std::unique_ptr<VKDevice> m_device;
        std::unique_ptr<VKSwapChain> m_swapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::unique_ptr<VKSimpleRenderSystem> m_simpleRenderSystem;
        //To keep track of the camera transform.
        VKGameObject m_cameraGameObject = VKGameObject::CreateGameObject();
        VKCamera m_camera;
        uint32_t m_currentImageIndex;
        bool m_isFrameStarted;
        bool m_needRecreateSwapChain = false;
    };
} // namespace LG
