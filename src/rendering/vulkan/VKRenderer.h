#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include "rendering/vulkan/VKPipeline.h"
#include "rendering/vulkan/VKSwapChain.h"
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
    private:
        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffers();
    private:
        EngineWindow* m_engineWindow = nullptr;
        std::unique_ptr<VKDevice> m_device = nullptr;
        std::unique_ptr<VKSwapChain> m_swapChain = nullptr;
        std::unique_ptr<VKPipeline> m_pipeline = nullptr;
        VkPipelineLayout m_pipelineLayout;
        std::vector<VkCommandBuffer> m_commandBuffers;
    };
} // namespace LG
