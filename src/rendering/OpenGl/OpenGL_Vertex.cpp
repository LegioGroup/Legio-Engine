#include "rendering/OpenGL/OpenGL_Vertex.h"

namespace LG
{
    Vertex::Vertex(glm::vec3 vertexPosition/*, glm::vec3 vertexColor, glm::vec2 texCords, glm::vec3 texNormals*/)
        : m_vertexPosition(vertexPosition)
        //, m_vertexColor(vertexColor)
        //, m_texCoords(texCords)
        //, m_texNormals(texNormals)
    {
    }

} //namespace LG 