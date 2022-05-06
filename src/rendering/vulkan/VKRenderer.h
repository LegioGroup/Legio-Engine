#pragma once
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Log.h>
#include "rendering/vulkan/VKPipeline.h"
#include "rendering/vulkan/VKSwapChain.h"
#include "rendering/vulkan/VKModel.h"

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
        void LoadModels();
    private:
        EngineWindow* m_engineWindow;
        std::unique_ptr<VKDevice> m_device;
        std::unique_ptr<VKSwapChain> m_swapChain;
        std::unique_ptr<VKPipeline> m_pipeline;
        VkPipelineLayout m_pipelineLayout;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::unique_ptr<VKModel> m_model;
    };
} // namespace LG
