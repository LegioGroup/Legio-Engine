#pragma once

#include "rendering/vulkan/VKModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace LG 
{

    struct TransformComponent 
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.f , 1.f, 1.f};
        glm::vec3 rotation;

        //Translate * Ry * Rx * Rz * Scale
        //Trait-bryan angles Y(1) X(2) Z(3)
        glm::mat4 mat4() 
        {
            //https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);

            return glm::mat4
            {
                {
                    scale.x * (c1 * c3 + s1 * s2 * s3),
                    scale.x * (c2 * s3),
                    scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0f
                },
                {
                    scale.y * (c3 * s1 * s2 - c1 * s3),
                    scale.y * (c2 * c3),
                    scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0f
                },
                {
                    scale.z * (c2 * s1),
                    scale.z * (-s2),
                    scale.z * (c1 * c2),
                    0.0f
                },
                {translation.x, translation.y, translation.z, 1.0f}
            };
        }
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
