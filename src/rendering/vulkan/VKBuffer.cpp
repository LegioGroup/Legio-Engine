#include "rendering/vulkan/VKBuffer.h"

/*
 * Encapsulates a vulkan m_buffer
 *
 * Initially based off VulkanBuffer by Sascha Willems -
 * https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanBuffer.h
 */

#include <cassert>
#include <cstring>

namespace LG {

    /**
     * Returns the minimum instance size required to be compatible with devices minOffsetAlignment
     *
     * @param m_instanceSize The size of an instance
     * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
     * minUniformBufferOffsetAlignment)
     *
     * @return VkResult of the m_buffer mapping call
     */
    VkDeviceSize VKBuffer::getAlignment(VkDeviceSize m_instanceSize, VkDeviceSize minOffsetAlignment) {
        if (minOffsetAlignment > 0) {
            return (m_instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }
        return m_instanceSize;
    }

    VKBuffer::VKBuffer(
        VKDevice& device,
        VkDeviceSize m_instanceSize,
        uint32_t m_instanceCount,
        VkBufferUsageFlags usageFlags,
        VkMemoryPropertyFlags m_memoryPropertyFlags,
        VkDeviceSize minOffsetAlignment)
        : m_device{ device },
        m_instanceSize{ m_instanceSize },
        m_instanceCount{ m_instanceCount },
        usageFlags{ usageFlags },
        m_memoryPropertyFlags{ m_memoryPropertyFlags } {
        m_alignmentSize = getAlignment(m_instanceSize, minOffsetAlignment);
        m_bufferSize = m_alignmentSize * m_instanceCount;
        device.CreateBuffer(m_bufferSize, usageFlags, m_memoryPropertyFlags, m_buffer, m_memory);
    }

    VKBuffer::~VKBuffer() {
        unmap();
        vkDestroyBuffer(m_device.GetDevice(), m_buffer, nullptr);
        vkFreeMemory(m_device.GetDevice(), m_memory, nullptr);
    }

    /**
     * Map a m_memory range of this m_buffer. If successful, m_mapped points to the specified m_buffer range.
     *
     * @param size (Optional) Size of the m_memory range to map. Pass VK_WHOLE_SIZE to map the complete
     * m_buffer range.
     * @param offset (Optional) Byte offset from beginning
     *
     * @return VkResult of the m_buffer mapping call
     */
    VkResult VKBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
        assert(m_buffer && m_memory && "Called map on m_buffer before create");
        return vkMapMemory(m_device.GetDevice(), m_memory, offset, size, 0, &m_mapped);
    }

    /**
     * Unmap a m_mapped m_memory range
     *
     * @note Does not return a result as vkUnmapMemory can't fail
     */
    void VKBuffer::unmap() {
        if (m_mapped) {
            vkUnmapMemory(m_device.GetDevice(), m_memory);
            m_mapped = nullptr;
        }
    }

    /**
     * Copies the specified data to the m_mapped m_buffer. Default value writes whole m_buffer range
     *
     * @param data Pointer to the data to copy
     * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete m_buffer
     * range.
     * @param offset (Optional) Byte offset from beginning of m_mapped region
     *
     */
    void VKBuffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(m_mapped && "Cannot copy to unmapped m_buffer");

        if (size == VK_WHOLE_SIZE) {
            memcpy(m_mapped, data, m_bufferSize);
        }
        else {
            char* memOffset = (char*)m_mapped;
            memOffset += offset;
            memcpy(memOffset, data, size);
        }
    }

    /**
     * Flush a m_memory range of the m_buffer to make it visible to the device
     *
     * @note Only required for non-coherent m_memory
     *
     * @param size (Optional) Size of the m_memory range to flush. Pass VK_WHOLE_SIZE to flush the
     * complete m_buffer range.
     * @param offset (Optional) Byte offset from beginning
     *
     * @return VkResult of the flush call
     */
    VkResult VKBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkFlushMappedMemoryRanges(m_device.GetDevice(), 1, &mappedRange);
    }

    /**
     * Invalidate a m_memory range of the m_buffer to make it visible to the host
     *
     * @note Only required for non-coherent m_memory
     *
     * @param size (Optional) Size of the m_memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
     * the complete m_buffer range.
     * @param offset (Optional) Byte offset from beginning
     *
     * @return VkResult of the invalidate call
     */
    VkResult VKBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkInvalidateMappedMemoryRanges(m_device.GetDevice(), 1, &mappedRange);
    }

    /**
     * Create a m_buffer info descriptor
     *
     * @param size (Optional) Size of the m_memory range of the descriptor
     * @param offset (Optional) Byte offset from beginning
     *
     * @return VkDescriptorBufferInfo of specified offset and range
     */
    VkDescriptorBufferInfo VKBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        return VkDescriptorBufferInfo{
            m_buffer,
            offset,
            size,
        };
    }

    /**
     * Copies "m_instanceSize" bytes of data to the m_mapped m_buffer at an offset of index * m_alignmentSize
     *
     * @param data Pointer to the data to copy
     * @param index Used in offset calculation
     *
     */
    void VKBuffer::writeToIndex(void* data, int index) {
        writeToBuffer(data, m_instanceSize, index * m_alignmentSize);
    }

    /**
     *  Flush the m_memory range at index * m_alignmentSize of the m_buffer to make it visible to the device
     *
     * @param index Used in offset calculation
     *
     */
    VkResult VKBuffer::flushIndex(int index) { return flush(m_alignmentSize, index * m_alignmentSize); }

    /**
     * Create a m_buffer info descriptor
     *
     * @param index Specifies the region given by index * m_alignmentSize
     *
     * @return VkDescriptorBufferInfo for instance at index
     */
    VkDescriptorBufferInfo VKBuffer::descriptorInfoForIndex(int index) {
        return descriptorInfo(m_alignmentSize, index * m_alignmentSize);
    }

    /**
     * Invalidate a m_memory range of the m_buffer to make it visible to the host
     *
     * @note Only required for non-coherent m_memory
     *
     * @param index Specifies the region to invalidate: index * m_alignmentSize
     *
     * @return VkResult of the invalidate call
     */
    VkResult VKBuffer::invalidateIndex(int index) {
        return invalidate(m_alignmentSize, index * m_alignmentSize);
    }

}  // namespace LG