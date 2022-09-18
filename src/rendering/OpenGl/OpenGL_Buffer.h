#pragma once
#include "rendering/OpenGL/OpenGL_Vertex.h"
#include <array>
#include <vector>
#include <memory>
#include <glad/glad.h>

namespace LG 
{
    class Shader;
    class Buffer 
    {
    public:
        Buffer(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Shader> shader);
        ~Buffer();
        void Draw();
    private:

        unsigned int m_EBO;
        unsigned int m_VAO;
        unsigned int m_VBO;
        std::shared_ptr<Shader> m_shader;
        std::vector<Vertex> m_vertices;
        size_t m_numIndices;
    };
} //namespace LG 