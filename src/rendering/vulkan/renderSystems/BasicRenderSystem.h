#pragma once

#include "rendering/vulkan/VKPipeline.h"
#include "rendering/vulkan/VKModel.h"
#include "rendering/vulkan/VKdevice.h"
#include "Legio/rendering/LGCamera.h"

namespace LG
{
    class BasicRenderSystem
    {
    public:
        BasicRenderSystem(VKDevice* device, VkRenderPass renderPass, LGCamera* camera);
        ~BasicRenderSystem();
        BasicRenderSystem(const BasicRenderSystem&) = delete;
        BasicRenderSystem& operator=(const BasicRenderSystem&) = delete;
        void Render(VkCommandBuffer commandBuffer);
    private:
        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);
    private:
        VKDevice* m_device;
        LGCamera* m_camera;
        std::unique_ptr<VKPipeline> m_pipeline;
        VKModel* m_model;
        VkPipelineLayout m_pipelineLayout;
    };
} // namespace LG
