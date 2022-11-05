#pragma once
#include <memory>
#include <glm/vec2.hpp>
namespace LG 
{

    class Texture 
    {
    public:
        static std::shared_ptr<Texture> Load(const char* filename);
        void Bind() const;
        Texture();
    
        inline uint32_t GetID() const { return m_id; }
    private:
        uint32_t m_id;
        glm::ivec2 m_size;
    };
} //namespace LG 
