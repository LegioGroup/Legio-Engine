#pragma once
#include <string>
#include <glm/glm.hpp>
#include <gl/gl.h>
namespace LG 
{
    class Shader 
    {
    public:
        Shader(std::string&& vertexLocation, std::string&& fragmentLocation);
        ~Shader();

        void Use();
        void SetupAttribs();

        void setInt(uint16_t loc, int val);
        void setFloat(uint16_t loc, float val);
        void setVec2(uint16_t loc, const glm::vec2& vec);
        void setVec3(uint16_t loc, const glm::vec3& vec);
        void setVec4(uint16_t loc, const glm::vec4& vec);
        void setMatrix(uint16_t loc, const glm::mat4& matrix);

    private:
        std::string ReadString(const std::string& filename);

    private:

        uint32_t m_programID;
        std::string m_vertex;
        std::string m_fragment;

        uint32_t m_posLoc;
        uint32_t m_colorLoc;
        uint32_t m_texCoordLoc;
    };
} //namespace LG