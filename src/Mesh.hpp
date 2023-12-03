#pragma once

#include <glm/glm.hpp>

#include "EventHandler.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
};

class Mesh : public EventObserver
{
public:
public:
    Mesh();
    Mesh(glm::vec3 position, glm::vec2 size, int length);

    void render();
private:
    uint32_t m_VAO;
    uint32_t m_VBO;
    uint32_t m_EBO;

    glm::vec3 m_Position;
    glm::vec2 m_Size;
    int m_Length;

    uint32_t m_VertexCount = 0;
    uint32_t m_IndexCount = 0;
private:
    void createMesh();
};
