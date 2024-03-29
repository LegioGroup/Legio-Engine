#pragma once
#include <vector>
#include "rendering/OpenGl/OpenGL_Vertex.h"
#include "rendering/OpenGl/OpenGL_Texture.h"

namespace LG 
{
    class Shader;
    class Mesh
    {

    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures);
        void Draw(Shader& shader) const;
    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<std::shared_ptr<Texture>> m_textures;

        unsigned int m_EBO;
        unsigned int m_VAO;
        unsigned int m_VBO;

        void Setup();
    };
}
