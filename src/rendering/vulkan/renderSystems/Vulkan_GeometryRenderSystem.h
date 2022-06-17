#pragma once

#include "rendering/vulkan/Vulkan_Pipeline.h"
#include "rendering/vulkan/Vulkan_Device.h"
#include "rendering/vulkan/Vulkan_GameObject.h"
#include "Legio/rendering/LGCamera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace LG
{

    class GeometryBasicRenderSystem
    {
    public:
        GeometryBasicRenderSystem(VKDevice* device, VkRenderPass renderPass, LGCamera* camera);
        ~GeometryBasicRenderSystem();
        GeometryBasicRenderSystem(const GeometryBasicRenderSystem&) = delete;
        GeometryBasicRenderSystem& operator=(const GeometryBasicRenderSystem&) = delete;
        void Render(VkCommandBuffer commandBuffer);
    private:
        void LoadGameObjects(); 
        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);
    private:
        VKDevice* m_device;
        LGCamera* m_camera;
        std::unique_ptr<VKPipeline> m_pipeline;
        std::vector<VKGameObject> m_gameObjects;

        VkPipelineLayout m_pipelineLayout;
    };
} // namespace LG
