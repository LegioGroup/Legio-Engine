#pragma once
#include <memory>
#include <glm/vec2.hpp>
namespace LG 
{

    class Texture 
    {
    public:
        static std::shared_ptr<Texture> Load(const char* filename);
        void Bind();
        Texture();
    
    private:
        uint32_t m_id;
        glm::ivec2 m_size;
    };
} //namespace LG 
