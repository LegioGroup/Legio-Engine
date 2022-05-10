#pragma once
#include "rendering/vulkan/VKPipeline.h"
#include "rendering/vulkan/VKdevice.h"
#include "rendering/vulkan/VKGameObject.h"
#include "rendering/vulkan/VKCamera.h"

#include <memory>
#include <vector>
namespace LG
{
    class VKSimpleRenderSystem
    {
    public:
        VKSimpleRenderSystem(VKDevice& device, VkRenderPass renderPass, VKCamera& camera);
        ~VKSimpleRenderSystem();
        VKSimpleRenderSystem(const VKSimpleRenderSystem&) = delete;
        VKSimpleRenderSystem& operator=(const VKSimpleRenderSystem&) = delete;

        void RenderGameObjects(VkCommandBuffer commandBuffer);

    private:
        void LoadGameObjects();
        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);
    private:
        VKDevice& m_device;
        VKCamera& m_camera;
        std::unique_ptr<VKPipeline> m_pipeline;
        VkPipelineLayout m_pipelineLayout;
        std::vector<VKGameObject> m_gameObjects;
    };

} 