#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "EventHandler.hpp"

#include <unordered_map>

class Camera : public EventObserver
{
public:
    Camera(glm::ivec2 windowSize);
    Camera(glm::ivec2 windowSize, glm::vec3 position);
    Camera(glm::ivec2 windowSize, glm::vec3 position, glm::vec3 lookat);
    Camera(glm::ivec2 windowSize, glm::vec3 position, double pitch, double yaw);

    ~Camera() {};

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void update(const UpdateEvent& event) override;
    void keyboardEvent(const KeyboardEvent& event) override;
    void mouseMoveEvent(const MouseMoveEvent& event) override;

    glm::vec3 getFront() { return m_Front; }
    glm::vec3 getRight() { return m_Right; }

    void setSensitivity(float sens) { m_Sensitivity = sens; }
    void setMovementSpeed(float speed) { m_MovementSpeed = speed; }

private:
    glm::ivec2 m_WindowSize;

    glm::vec3 m_Position;
    double m_Pitch;
    double m_Yaw;

    float m_Zoom = 90.0f;
    float m_NearClipping = 0.01f;
    float m_FarClipping = 100.0f;

    float m_Sensitivity = 10.0f;
    float m_MovementSpeed = 10.0f;

    glm::vec3 m_Front;
    glm::vec3 m_Right;
    glm::vec3 m_Up;

    glm::vec3 m_WorldUp { 0.0f, 1.0f, 0.0f };
    glm::vec3 m_WorldFront { 0.0f, 0.0f, 1.0f };
    glm::vec3 m_WorldRight { 1.0f, 0.0f, 0.0f };


    std::unordered_map<int32_t, bool> m_PressedKeys;
    float m_TotalOffsetX = 0.0f;
    float m_TotalOffsetY = 0.0f;
private:
    void calculateVectors();
};