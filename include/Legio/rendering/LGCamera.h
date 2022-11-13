#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_ONE
#include <glm/glm.hpp>
#include <Legio/application/Events/Event.h>
#include <Legio/world/components/TransformComponent.h>

namespace LG
{
    class LGCamera
    {
    public:
        LGCamera() = default;
        void SetOrthographicProjection(float left, float right, float top, float bottom, float nearP, float farP);
        void SetPerspectiveProjection(float fovy, float aspect, float nearP, float farP);
        void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
        void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
        void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

        inline float GetMoveSpeed() const { return m_cameraData.m_moveSpeed; }
        inline float GetLookSpeed() const { return m_cameraData.m_lookSpeed; }

        inline const glm::mat4& GetProjection() const { return m_projectionMatrix; }
        inline const glm::mat4& GetView() const { return m_viewMatrix; }

        bool OnAppTickEvent(AppTickEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);

        World::TransformComponent GeTransform() { return m_transform; }
        glm::vec3 GetCameraFront() const { return m_cameraFront; }
    private:
        glm::mat4 m_projectionMatrix{ 1.f };
        glm::mat4 m_viewMatrix{ 1.f };

        glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        float m_fov = 0.f;
        World::TransformComponent m_transform;

        float m_mouseSensitivity = 0.00001f;
        float m_pitch;
        float m_yaw = -90.f;
        float m_lastMouseXPosition = 0;
        float m_lastMouseYPosition = 0;

        struct CameraMovementData
        {
            float m_moveSpeed = 10.0f;
            float m_moveSpeedShift = 20.0f;
            float m_lookSpeed = 1.5f;
        };

        CameraMovementData m_cameraData;
    };
} //namespace LG
