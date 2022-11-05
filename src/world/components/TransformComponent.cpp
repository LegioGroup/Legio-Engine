#include <Legio/world/components/TransformComponent.h>

namespace LG::World
{
    void TransformComponent::Translate(glm::vec3 translation)
    {
        m_transform = glm::translate(m_transform, translation);
    }

    void TransformComponent::Rotate(float degrees, Axis rotationAxis)
    {
        glm::vec3 axis;
        switch (rotationAxis)
        {
        case Axis::X:
            axis = glm::vec3(1.f, 0.f, 0.f);
            break;
        case Axis::Y:
            axis = glm::vec3(0.f, 1.f, 0.f);
            break;
        case Axis::Z:
            axis = glm::vec3(0.f, 0.f, 1.f);
            break;
        }

        m_transform = glm::rotate(m_transform,glm::radians(degrees), axis);
    }

    void TransformComponent::Scale(glm::vec3 newScale)
    {
        m_transform = glm::scale(m_transform, newScale);
    }

    glm::vec3 TransformComponent::GetPositon() const
    {
        Decompose();
        return m_position;
    }

    glm::mat4 TransformComponent::GetTransform() const
    {
        return m_transform;
    }

    glm::vec3 TransformComponent::GetScale() const
    {
        Decompose();
        return m_scale;
    }

    glm::quat TransformComponent::GetRotation() const
    {
        Decompose();
        return m_rotation;
    }

    glm::vec3 TransformComponent::GetRoatationEuler() const
    {
        Decompose();
        return glm::eulerAngles(m_rotation);
    }

    void TransformComponent::Decompose() const
    {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_transform, m_scale, m_rotation, m_position, skew, perspective);
    }
}