#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
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

        stbi_set_flip_vertically_on_load(true);
        unsigned char* image = stbi_load(filename, &texture->m_size.x, &texture->m_size.y, nullptr, 4);

        if(image)
        {
            glGenTextures(1, &texture->m_id);
            glBindTexture(GL_TEXTURE_2D, texture->m_id);

            texture->Bind();
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->m_size.x, texture->m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); //Carga imagen

            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(image);
        }

        return texture;

    }
    
    void Texture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
} //namespace LG
