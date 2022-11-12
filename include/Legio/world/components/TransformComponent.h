#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace LG::World
{
    class TransformComponent
    {
    public:
        TransformComponent() = default;

        void Translate(glm::vec3 translation);
        void Rotate(float degrees, const glm::vec3& rotationAxis);
        void Scale(glm::vec3 newScale);

        glm::mat4 GetTransform() const;
        glm::vec3 GetPositon() const;
        glm::vec3 GetScale() const;
        glm::quat GetRotation() const;
        glm::vec3 GetRoatationEuler() const;

    private:
        void Decompose() const;
    private:
        mutable glm::mat4 m_transform = glm::mat4(1.0f);
        mutable glm::vec3 m_position{};
        mutable glm::vec3 m_scale{};
        mutable glm::quat m_rotation{};
    };
}