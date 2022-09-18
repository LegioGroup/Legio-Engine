
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <Legio/platform/Log.h>
#include "rendering/OpenGL/OpenGL_Shader.h"
#include "rendering/OpenGL/OpenGL_Vertex.h"

namespace LG
{
    Shader::Shader(std::string&& vertexLocation, std::string&& fragmentLocation)
        : m_vertex(ReadString(vertexLocation))
        , m_fragment(ReadString(fragmentLocation))
    {
        //Shader Program
        unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);

        const char* vertexSource = m_vertex.c_str();
        glShaderSource(vertexID, 1, &vertexSource, NULL);
        glCompileShader(vertexID);

        int  success;
        char infoLog[512];
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
            LG_CORE_ERROR("SHADER::VERTEX::COMPILATION_FAILED\n");
        }

        unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSource = m_fragment.c_str();
        glShaderSource(fragmentID, 1, &fragmentSource, NULL);
        glCompileShader(fragmentID);

        m_programID = glCreateProgram();

        glAttachShader(m_programID, vertexID);
        glAttachShader(m_programID, fragmentID);
        glLinkProgram(m_programID);

        glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
            LG_CORE_ERROR("SHADER::FRAGMENT::COMPILATION_FAILED\n");
        }
        glDetachShader(m_programID, vertexID);
        glDetachShader(m_programID, fragmentID);

        m_posLoc = glGetAttribLocation(m_programID, "vpos");
        m_colorLoc = glGetAttribLocation(m_programID, "vcolor");
        m_texCoordLoc = glGetAttribLocation(m_programID, "vtex");

        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_programID);
    }

    void Shader::Use()
    {
        glUseProgram(m_programID);
    }

    void Shader::SetupAttribs()
    {
        //glEnableVertexAttribArray(m_posLoc);
        //glVertexAttribPointer(m_posLoc, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(m_posLoc);
        glVertexAttribPointer(m_posLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, m_vertexPosition)));

        /*glEnableVertexAttribArray(m_colorLoc);
        glVertexAttribPointer(m_colorLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, m_vertexColor)));

        glEnableVertexAttribArray(m_texCoordLoc);
        glVertexAttribPointer(m_texCoordLoc, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, m_texCoords)));*/

    }

    void Shader::setInt(uint16_t loc, int val)
    {
        glUniform1i(loc, val);
    }

    void Shader::setFloat(uint16_t loc, float val)
    {
        glUniform1f(loc, val);
    }

    void Shader::setVec2(uint16_t loc, const glm::vec2& vec)
    {
        glUniform2f(loc, vec.x, vec.y);
    }

    void Shader::setVec3(uint16_t loc, const glm::vec3& vec)
    {
        glUniform3f(loc, vec.x, vec.y, vec.z);
    }

    void Shader::setVec4(uint16_t loc, const glm::vec4& vec)
    {
        glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
    }

    void Shader::setMatrix(uint16_t loc, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    std::string Shader::ReadString(const std::string& filename)
    {
        std::ifstream istream(filename.c_str(), std::ios_base::binary);
        std::stringstream sstream;
        sstream << istream.rdbuf();

        return sstream.str();
    }

} //namespace LG
