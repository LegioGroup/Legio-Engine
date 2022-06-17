#include "rendering/vulkan/Vulkan_Implementation.h"
#include "rendering/vulkan/Vulkan_Model.h"

namespace LG
{
    VkVertexInputBindingDescription VKModel::Vertex::GetBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 2> VKModel::Vertex::GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }

    VKModel::VKModel(VKDevice* device)
        : m_device(device)
    {
        CreateVertexBuffer();
        if(HasIndices())
        {
            CreateIndexBuffer();
        }
    }

    VKModel::~VKModel()
    {
        vkDestroyBuffer(m_device->GetDevice(), m_vertexBuffer, nullptr);
        vkFreeMemory(m_device->GetDevice(), m_vertexBufferMemory, nullptr);

        if(HasIndices())
        {
            vkDestroyBuffer(m_device->GetDevice(), m_indexBuffer, nullptr);
            vkFreeMemory(m_device->GetDevice(), m_indexBufferMemory, nullptr);
        }
     }

    void VKModel::CreateVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VulkanUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(m_device->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, m_vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(m_device->GetDevice(), stagingBufferMemory);

        VulkanUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);
        VulkanUtils::CopyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

        vkDestroyBuffer(m_device->GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(m_device->GetDevice(), stagingBufferMemory, nullptr);
    }

    void VKModel::CreateIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(m_inidices[0]) * m_inidices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VulkanUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data = nullptr;
        VulkanUtils::RetrieveBufferData(bufferSize, (void*)m_inidices.data(), stagingBufferMemory, data);

        VulkanUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

        VulkanUtils::CopyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

        vkDestroyBuffer(m_device->GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(m_device->GetDevice(), stagingBufferMemory, nullptr);
    }

    void VKModel::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer vertexBuffers[] = { m_vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        if(HasIndices())
        {
            vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);
        }
    }

    void VKModel::Draw(VkCommandBuffer commandBuffer)
    {
        if(HasIndices())
        {
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_inidices.size()), 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0);
        }
    }
} // namespace LG
