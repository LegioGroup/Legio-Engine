#include <Legio/rendering/LGCamera.h>
#include <cassert>
#include <limits>
#include <Legio/ServiceLocator.h>

namespace LG
{
    void LGCamera::SetOrthographicProjection(float left, float right, float top, float bottom, float nearP, float farP)
    {
        m_projectionMatrix = glm::mat4{ 1.0f };
        m_projectionMatrix[0][0] = 2.f / (right - left);
        m_projectionMatrix[1][1] = 2.f / (bottom - top);
        m_projectionMatrix[2][2] = 1.f / (farP - nearP);
        m_projectionMatrix[3][0] = -(right + left) / (right - left);
        m_projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        m_projectionMatrix[3][2] = -nearP / (farP - nearP);
    }
    
    void LGCamera::SetPerspectiveProjection(float fovy, float aspect, float nearP, float farP)
    {
        m_fov = glm::degrees(fovy);
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovy = tan(fovy / 2.f);
        m_projectionMatrix = glm::mat4{ 0.0f };
        m_projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
        m_projectionMatrix[1][1] = 1.f / (tanHalfFovy);
        m_projectionMatrix[2][2] = farP / (farP - nearP);
        m_projectionMatrix[2][3] = 1.f;
        m_projectionMatrix[3][2] = -(farP * nearP) / (farP - nearP);
    }

    void LGCamera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
    {
        const glm::vec3 w{ glm::normalize(direction) };
        const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
        const glm::vec3 v{ glm::cross(w, u) };

        m_viewMatrix = glm::mat4{ 1.f };
        m_viewMatrix[0][0] = u.x;
        m_viewMatrix[1][0] = u.y;
        m_viewMatrix[2][0] = u.z;
        m_viewMatrix[0][1] = v.x;
        m_viewMatrix[1][1] = v.y;
        m_viewMatrix[2][1] = v.z;
        m_viewMatrix[0][2] = w.x;
        m_viewMatrix[1][2] = w.y;
        m_viewMatrix[2][2] = w.z;
        m_viewMatrix[3][0] = -glm::dot(u, position);
        m_viewMatrix[3][1] = -glm::dot(v, position);
        m_viewMatrix[3][2] = -glm::dot(w, position);
    }

    void LGCamera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    {
        SetViewDirection(position, target - position, up);
    }

    void LGCamera::SetViewYXZ(glm::vec3 position, glm::vec3 rotation)
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
        const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
        const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
        m_viewMatrix = glm::mat4{ 1.f };
        m_viewMatrix[0][0] = u.x;
        m_viewMatrix[1][0] = u.y;
        m_viewMatrix[2][0] = u.z;
        m_viewMatrix[0][1] = v.x;
        m_viewMatrix[1][1] = v.y;
        m_viewMatrix[2][1] = v.z;
        m_viewMatrix[0][2] = w.x;
        m_viewMatrix[1][2] = w.y;
        m_viewMatrix[2][2] = w.z;
        m_viewMatrix[3][0] = -glm::dot(u, position);
        m_viewMatrix[3][1] = -glm::dot(v, position);
        m_viewMatrix[3][2] = -glm::dot(w, position);
    }

    bool LGCamera::OnAppTickEvent(AppTickEvent& event)
    {
        static float fixedTick = event.GetFixedTick();
        static float moveSpeed = 0;
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_LEFT_SHIFT))
        {
            moveSpeed = fixedTick * m_cameraData.m_moveSpeedShift;
        }
        else 
        {
            moveSpeed = fixedTick * m_cameraData.m_moveSpeed;
        }

        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_W))  m_transform.Translate( moveSpeed * m_cameraFront);
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_S))  m_transform.Translate(-moveSpeed * m_cameraFront);
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_A))  m_transform.Translate(-moveSpeed * glm::normalize(glm::cross(m_cameraFront, m_cameraUp)));
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_D))  m_transform.Translate( moveSpeed * glm::normalize(glm::cross(m_cameraFront, m_cameraUp)));

        SetViewDirection(m_transform.GetPositon(), m_cameraFront, m_cameraUp);
        return false;
    }

    bool LGCamera::OnMouseMovedEvent(MouseMovedEvent& event)
    {
        float mouseX = event.GetX();
        float mouseY = event.GetY();

        static bool firstMouse = true;
        if (firstMouse)
        {
            m_lastMouseXPosition = mouseX;
            m_lastMouseYPosition = mouseY;
            firstMouse = false;
        }

        float xOffset = mouseX - m_lastMouseXPosition;
        float yOffset = mouseY - m_lastMouseYPosition;

        m_lastMouseXPosition = mouseX;
        m_lastMouseYPosition = mouseY;

        xOffset += m_mouseSensitivity;
        yOffset += m_mouseSensitivity;

        m_yaw += xOffset * 0.1f;
        m_pitch += yOffset * 0.1f;

        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        direction.y = sin(glm::radians(m_pitch));
        direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_cameraFront = glm::normalize(direction);

        return false;
    }

    bool LGCamera::OnMouseScrolledEvent(MouseScrolledEvent& event)
    {
        m_fov -= event.GetYOffset();
        if (m_fov < 1.0f) 
        {
            m_fov = 1.0f;
        }
        if (m_fov > 100.f)
        {
            m_fov = 100.f;
        }
        SetPerspectiveProjection(glm::radians(m_fov), LG::ServiceLocator::GetWindow()->GetWidth() / LG::ServiceLocator::GetWindow()->GetHeight(), 0.1f, 10000.f);
        return false;
    }

} // namespace LG
