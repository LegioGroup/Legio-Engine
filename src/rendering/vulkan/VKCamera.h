#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_ONE
#include <glm/glm.hpp>

namespace LG
{
    class VKCamera
    {
    public:
        VKCamera() = default;
        void SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void SetPerspectiveProjection(float fovy, float aspect, float near, float far);
        void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
        void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
        void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

        inline float GetMoveSpeed() const { return m_cameraData.m_moveSpeed; }
        inline float GetLookSpeed() const { return m_cameraData.m_lookSpeed; }

        inline const glm::mat4& GetProjection() const { return m_projectionMatrix; }
        inline const glm::mat4& GetView() const { return m_viewMatrix; }
    private:
        glm::mat4 m_projectionMatrix{ 1.f };
        glm::mat4 m_viewMatrix{ 1.f };

        struct CameraMovementData
        {
            float m_moveSpeed = 3.0f;
            float m_lookSpeed = 1.5f;
        };

        CameraMovementData m_cameraData;
    };
} //namespace LG
