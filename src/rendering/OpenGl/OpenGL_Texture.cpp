#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <Legio/platform/Log.h>
#include "rendering/OpenGL/OpenGL_Texture.h"

namespace LG 
{

    Texture::Texture()
        : m_id(0)
        , m_size(glm::ivec2(0))
    {
    }

    std::shared_ptr<Texture> Texture::Load(const char* filename)
    {
        std::shared_ptr<Texture> texture = std::make_shared<Texture>();
        texture->m_path = filename;

        int nrComponents;
        stbi_set_flip_vertically_on_load(true);

        unsigned char* image = stbi_load(filename, &texture->m_size.x, &texture->m_size.y, &nrComponents, 0);
        if (!image) 
        {
            LG_CORE_ERROR("Texture::Load - Cannot load file {0}", filename);
            return texture;
        }

        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glGenTextures(1, &texture->m_id);
        glBindTexture(GL_TEXTURE_2D, texture->m_id);

        texture->Bind();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, format, texture->m_size.x, texture->m_size.y, 0, format, GL_UNSIGNED_BYTE, image); //Load imagen

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);

        return texture;

    }

    void Texture::Bind() const
    {
        glActiveTexture(GL_TEXTURE0 + m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
} //namespace LG
