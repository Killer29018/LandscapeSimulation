#include "Camera.hpp"

#include "GLFW/glfw3.h"
#include <algorithm>
#include <unordered_map>

Camera::Camera(glm::ivec2 windowSize)
    : m_WindowSize(windowSize), m_Position({ 0.0f, 0.0f, 0.0f }), m_Pitch(0.0f), m_Yaw(0.0f)
{
    calculateVectors();
}

Camera::Camera(glm::ivec2 windowSize, glm::vec3 position)
    : m_WindowSize(windowSize), m_Position(position), m_Pitch(0.0f), m_Yaw(0.0f)
{
    calculateVectors();
}

Camera::Camera(glm::ivec2 windowSize, glm::vec3 position, glm::vec3 lookat)
    : m_WindowSize(windowSize), m_Position(position)
{
    glm::vec3 direction = lookat - position;

    m_Yaw = std::atan2(direction.x, m_WorldFront.z);

    calculateVectors();
}

Camera::Camera(glm::ivec2 windowSize, glm::vec3 position, double pitch, double yaw)
    : m_WindowSize(windowSize), m_Position(position), m_Pitch(pitch), m_Yaw(yaw)
{
    calculateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(m_Zoom), (float)m_WindowSize.x / (float)m_WindowSize.y, m_NearClipping, m_FarClipping);
}

void Camera::update(const UpdateEvent& event)
{
    m_Pitch += m_TotalOffsetY * event.dt * m_Sensitivity;
    m_Yaw += m_TotalOffsetX * event.dt * m_Sensitivity;

    m_TotalOffsetX = 0.0f;
    m_TotalOffsetY = 0.0f;

    m_Pitch = std::clamp(m_Pitch, -90.0, 90.0);

    for (auto it = m_PressedKeys.begin(); it!= m_PressedKeys.end(); it++)
    {
        if (it->second)
        {
            switch (it->first)
            {
                case GLFW_KEY_W:
                    m_Position += m_Front * event.dt * m_MovementSpeed;
                    break;
                case GLFW_KEY_A:
                    m_Position -= m_Right * event.dt * m_MovementSpeed;
                    break;
                case GLFW_KEY_S:
                    m_Position -= m_Front * event.dt * m_MovementSpeed;
                    break;
                case GLFW_KEY_D:
                    m_Position += m_Right * event.dt * m_MovementSpeed;
                    break;
                case GLFW_KEY_SPACE:
                    m_Position += m_WorldUp * event.dt * m_MovementSpeed;
                    break;
                case GLFW_KEY_LEFT_CONTROL:
                    m_Position -= m_WorldUp * event.dt * m_MovementSpeed;
                    break;
            }
        }
    }

    calculateVectors();
}

void Camera::keyboardEvent(const KeyboardEvent& event)
{
    if (event.action == GLFW_PRESS)
    {
        m_PressedKeys[event.key] = true;
    }
    else if (event.action == GLFW_RELEASE)
    {
        m_PressedKeys[event.key] = false;
    }
}

void Camera::mouseMoveEvent(const MouseMoveEvent& event)
{
    m_TotalOffsetX += event.xOffset;
    m_TotalOffsetY += event.yOffset;
}

void Camera::calculateVectors()
{
    float yaw = m_Yaw - 90.0f;
    float pitch = m_Pitch;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    m_Front = glm::normalize(direction);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
