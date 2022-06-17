#pragma once

#include "rendering/vulkan/Vulkan_Device.h"
namespace LG
{
    namespace VulkanUtils
    {
        struct Globals
        {
            static inline VKDevice* device;
        };

        static inline VkCommandBuffer BeginSingleTimeCommand()
        {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
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

        void inline EndSingleTimeCommandBuffer(VkCommandBuffer commandBuffer)
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

    } // namespace VulkanUtils
} // namespace VulkanUtils
