#pragma once
#include <stdexcept>
#include "rendering/vulkan/Vulkan_Device.h"
namespace LG
{
    namespace VulkanUtils
    {
        struct Globals
        {
            static inline VKDevice* device;
        };

        static inline VkCommandBuffer BeginSingleTimeCommands()
        {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = Globals::device->GetCommandPool();
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(Globals::device->GetDevice(), &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &beginInfo);

            return commandBuffer;
        }

        static inline void EndSingleTimeCommands(VkCommandBuffer commandBuffer)
        {
            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(Globals::device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(Globals::device->GetGraphicsQueue());

            vkFreeCommandBuffers(Globals::device->GetDevice(),  Globals::device->GetCommandPool(), 1, &commandBuffer);
        }


        static inline void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
        {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(Globals::device->GetDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create buffer!");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(Globals::device->GetDevice(), buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = Globals::device->FindMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(Globals::device->GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to allocate buffer memory!");
            }

            vkBindBufferMemory(Globals::device->GetDevice(), buffer, bufferMemory, 0);
        }

        static inline void CopyBuffer(VkBuffer srcBuffer, VkBuffer destBuffer, VkDeviceSize size)
        {
            VkCommandBuffer commandBuffer = VulkanUtils::BeginSingleTimeCommands();

            VkBufferCopy copyRegion{};
            copyRegion.srcOffset = 0; // Optional
            copyRegion.dstOffset = 0; // Optional
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer, srcBuffer, destBuffer, 1, &copyRegion);

            VulkanUtils::EndSingleTimeCommands(commandBuffer);
        }

        static inline void RetrieveBufferData(VkDeviceSize bufferSize, void* srcData, VkDeviceMemory bufferMemory, void* destData)
        {
            vkMapMemory(Globals::device->GetDevice(), bufferMemory, 0, bufferSize, 0, &destData);
            memcpy(destData, srcData, (size_t) bufferSize);
            vkUnmapMemory(Globals::device->GetDevice(), bufferMemory);
        }
    } // namespace VulkanUtils
} // namespace VulkanUtils
