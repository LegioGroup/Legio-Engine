#pragma once
#include <vulkan/vulkan.h>
#include <string>

class VKDevice;
namespace LG
{
    class VKTexture
    {
    public:
        VKTexture(VKDevice* device, std::string&& filepath);
        ~VKTexture();

        VKTexture(const VKTexture&) = delete;
        VKTexture(VKTexture&&) = delete;

        VKTexture& operator=(const VKTexture&) = delete;
        VKTexture& operator=(VKTexture&&) = delete;

    private:
        void CreateTexture(std::string&& filePath);
        void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
            VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory imageMemory);
        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    private:
        VKDevice* m_device;
        VkImage m_textureImage;
        VkDeviceMemory m_textureImageMemory;
    };
} // namespace LG
