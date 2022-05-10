#pragma once

#include "rendering/vulkan/VKDevice.h"

namespace LG {

    class VKBuffer {
    public:
        VKBuffer(
            VKDevice& device,
            VkDeviceSize m_instanceSize,
            uint32_t m_instanceCount,
            VkBufferUsageFlags usageFlags,
            VkMemoryPropertyFlags m_memoryPropertyFlags,
            VkDeviceSize minOffsetAlignment = 1);
        ~VKBuffer();

        VKBuffer(const VKBuffer&) = delete;
        VKBuffer& operator=(const VKBuffer&) = delete;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void unmap();

        void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void writeToIndex(void* data, int index);
        VkResult flushIndex(int index);
        VkDescriptorBufferInfo descriptorInfoForIndex(int index);
        VkResult invalidateIndex(int index);

        VkBuffer getBuffer() const { return m_buffer; }
        void* getMappedMemory() const { return m_mapped; }
        uint32_t getInstanceCount() const { return m_instanceCount; }
        VkDeviceSize getInstanceSize() const { return m_instanceSize; }
        VkDeviceSize getAlignmentSize() const { return m_instanceSize; }
        VkBufferUsageFlags getUsageFlags() const { return usageFlags; }
        VkMemoryPropertyFlags getMemoryPropertyFlags() const { return m_memoryPropertyFlags; }
        VkDeviceSize getBufferSize() const { return m_bufferSize; }

    private:
        static VkDeviceSize getAlignment(VkDeviceSize m_instanceSize, VkDeviceSize minOffsetAlignment);

        VKDevice& m_device;
        void* m_mapped = nullptr;
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_memory = VK_NULL_HANDLE;

        VkDeviceSize m_bufferSize;
        uint32_t m_instanceCount;
        VkDeviceSize m_instanceSize;
        VkDeviceSize m_alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags m_memoryPropertyFlags;
    };

}  // namespace LG