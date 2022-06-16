#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include "rendering/vulkan/VKDevice.h"

namespace LG
{
    struct UBO
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    class VKModel
    {
    public:
        struct Vertex
        {
            glm::vec2 pos;
            glm::vec3 color;

            static VkVertexInputBindingDescription GetBindingDescription();
            static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
        };

        VKModel(VKDevice* device);
        ~VKModel();

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

        inline const std::vector<Vertex>& GetVertices() const { return m_vertices; }

    private:
        void CreateVertexBuffer();
        void CreateIndexBuffer();

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer destBuffer, VkDeviceSize size);
        inline bool HasIndices() { return m_inidices.size() > 0; }
    private:
        VKDevice* m_device;

        VkBuffer m_vertexBuffer;
        VkDeviceMemory m_vertexBufferMemory;

        VkBuffer m_indexBuffer;
        VkDeviceMemory m_indexBufferMemory;

        const std::vector<Vertex> m_vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };

        const std::vector<uint16_t> m_inidices = {
            0, 1, 2, 2, 3, 0
        };

    };
} // namespace LG
