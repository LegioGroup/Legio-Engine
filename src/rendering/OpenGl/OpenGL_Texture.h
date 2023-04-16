#pragma once
#include <memory>
#include <string>
#include <glm/vec2.hpp>
namespace LG 
{

    class Texture 
    {
    public:
        static std::shared_ptr<Texture> Load(const char* filename);
        void Bind() const;
        Texture();
    
        inline void SetType(std::string newType) { m_type = std::move(newType); }
        inline uint32_t GetID() const { return m_id; }
        inline std::string GetType() const { return m_type; }
        inline std::string GetPath() const { return m_path; }

    private:
        std::string m_type;
        std::string m_path;
        uint32_t m_id;
        glm::ivec2 m_size;
    };
} //namespace LG 
