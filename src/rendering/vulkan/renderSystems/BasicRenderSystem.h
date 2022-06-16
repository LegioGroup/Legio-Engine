#pragma once

#include "rendering/vulkan/VKPipeline.h"
#include "rendering/vulkan/VKdevice.h"
#include "rendering/vulkan/VKGameObject.h"
#include "Legio/rendering/LGCamera.h"
#include <glm/gtc/matrix_transform.hpp>
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
