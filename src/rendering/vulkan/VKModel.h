#pragma once

#include "rendering/vulkan/VKDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_ONE
#include <glm/glm.hpp>
#include <vector>
namespace LG {

    class VKModel
    {
    public:

        struct Vertex 
        {
            glm::vec2 position;
            static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> GetAttributesDescriptions();
        };

        VKModel(VKDevice& device, const std::vector<Vertex>& vertices);
        ~VKModel();

        VKModel(const VKModel&) = delete;
        VKModel& operator=(const VKModel&) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

    private:
        void CreateVertexBuffers(const std::vector<Vertex>& vertices);

    private:
        VKDevice& m_device;
        VkBuffer m_vertexBuffer;
        VkDeviceMemory m_vertexBufferMemory;
        uint32_t m_vertexCount;
    };
}