#include "rendering/OpenGL/OpenGL_Buffer.h"
#include <rendering/OpenGL/OpenGL_Shader.h>

namespace LG
{


    Buffer::Buffer(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Shader> shader)
        : m_vertices(vertices)
        , m_shader(shader)
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

        m_shader->SetupAttribs();
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_numIndices = 6;/*m_indices.size()*/;
    }

    Buffer::~Buffer()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Buffer::Draw()
    {
        m_shader->Use();

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        m_shader->SetupAttribs();

        if (m_numIndices > 0)
        {
            glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
        }
        else 
        {
            glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

        }
    }



} //namespace LG 