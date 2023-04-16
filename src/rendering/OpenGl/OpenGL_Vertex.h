#pragma once
#include <glm/glm.hpp>
namespace LG
{
    class Vertex 
    {
    friend class Shader;
    friend class Mesh;
    public:
        Vertex(glm::vec3 vertexPosition, glm::vec3 vertexColor, glm::vec2 texCords, glm::vec3 texNormals, glm::vec3 tangents = {}, glm::vec3 biTangents = {});

    private:
        glm::vec3 m_vertexPosition;
        glm::vec3 m_vertexColor;
        glm::vec2 m_texCoords;
        glm::vec3 m_texNormals;
        glm::vec3 m_tangents;
        glm::vec3 m_biTangents;

    };
} //namespace LG 