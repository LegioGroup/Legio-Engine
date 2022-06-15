#include "rendering/vulkan/VKModel.h"
#include <stdexcept>
 
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
    }

    VKModel::~VKModel()
    {
        vkDestroyBuffer(m_device->GetDevice(), m_vertexBuffer, nullptr);
        vkFreeMemory(m_device->GetDevice(), m_vertexBufferMemory, nullptr);

    }

    void VKModel::CreateVertexBuffer()
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(m_vertices[0]) * m_vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_device->GetDevice(), &bufferInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_device->GetDevice(), m_vertexBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = m_device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if(vkAllocateMemory(m_device->GetDevice(), &allocInfo, nullptr, &m_vertexBufferMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(m_device->GetDevice(), m_vertexBuffer, m_vertexBufferMemory, 0);

        void* data;
        vkMapMemory(m_device->GetDevice(), m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
            memcpy(data, m_vertices.data(), (size_t) bufferInfo.size);
        vkUnmapMemory(m_device->GetDevice(), m_vertexBufferMemory);
    }

    void VKModel::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer vertexBuffers[] = {m_vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    }

    void VKModel::Draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0);
    }
} // namespace LG
