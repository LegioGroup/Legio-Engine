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

        void Use() const;
        void SetupAttribs() const;

        void setInt(const char* loc, int val);
        void setFloat(const char* loc, float val);
        void setVec2(const char* loc, const glm::vec2& vec);
        void setVec3(const char* loc, const glm::vec3& vec);
        void setVec4(const char* loc, const glm::vec4& vec);
        void setMatrix(const char* loc, const glm::mat4& matrix);

        inline uint32_t GetId() const { return m_programID; }
        inline uint16_t GetLocation(const char* locationName) const { return glGetUniformLocation(m_programID, locationName); }

    private:
        void setInt(uint16_t loc, int val) const;
        void setFloat(uint16_t loc, float val) const;
        void setVec2(uint16_t loc, const glm::vec2& vec) const;
        void setVec3(uint16_t loc, const glm::vec3& vec) const;
        void setVec4(uint16_t loc, const glm::vec4& vec) const;
        void setMatrix(uint16_t loc, const glm::mat4& matrix) const;

        std::string ReadString(std::string filename);

    private:

        uint32_t m_programID;
        std::string m_vertex;
        std::string m_fragment;

        uint32_t m_posLoc;
        uint32_t m_texCoordLoc;
        uint32_t m_normalCoords;
    };
} //namespace LG