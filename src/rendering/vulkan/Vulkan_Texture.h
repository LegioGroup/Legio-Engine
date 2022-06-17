#pragma once
#include <stb_image.h>
namespace LG
{
    class VKTexture
    {
        VKTexture() = default;
        ~VKTexture() =  default;

        VKTexture(const VKTexture&) = delete;
        VKTexture(VKTexture&&) = delete;

        VKTexture& operator=(const VKTexture&) = delete;
        VKTexture& operator=(VKTexture&&) = delete;
    };
} // namespace LG
