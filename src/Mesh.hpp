#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "EventHandler.hpp"
#include "Shader.hpp"

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
    Mesh(glm::vec3 position, glm::vec2 size, int triangleSide);

    void regenerate() { generateHeight(); }
    void render(const RenderEvent& event) override;
private:
    uint32_t m_VAO;
    uint32_t m_VBO;
    uint32_t m_EBO;

    uint32_t m_HeightTexture;
    glm::vec2 m_HeightTextureSize;

    Shader m_Shader;

    glm::vec3 m_Position;
    glm::vec2 m_Size;
    int m_Length;

    uint32_t m_VertexCount = 0;
    uint32_t m_IndexCount = 0;
private:
    void generateData();
    void generateMesh();
    void generateHeight();
};
