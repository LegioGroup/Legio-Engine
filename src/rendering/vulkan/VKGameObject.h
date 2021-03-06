#pragma once

#include "rendering/vulkan/VKModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace LG 
{

    struct TransformComponent 
    {
        glm::vec3 translation{0.f};
        glm::vec3 scale{1.f , 1.f, 1.f};
        glm::vec3 rotation{0.f};

        glm::mat4 Mat4();
        glm::mat3 NormalMatrix();
    };
    class VKGameObject
    {
    public:
        using id_t = unsigned int;

        VKGameObject(const VKGameObject&) = delete;
        VKGameObject& operator=(const VKGameObject&) = delete;

        VKGameObject(VKGameObject&&) = default;
        VKGameObject& operator=(VKGameObject&&) = default;


        static VKGameObject CreateGameObject() 
        {
            static id_t currentId = 0;
            return VKGameObject(currentId++);
        }

        id_t GetId() { return m_id; };

    public:
    
        std::shared_ptr<VKModel> m_model;
        glm::vec3 m_color{};
        TransformComponent m_transformComponent{};

    private:
        VKGameObject(id_t objId) : m_id(objId) {};

    private:
        id_t m_id;

    };
}
