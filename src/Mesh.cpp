#include "Mesh.hpp"
#include "glad/gl.h"

#include <vector>

Mesh::Mesh()
    : m_Position(0.0f), m_Size(0.0f), m_Length(0)
{

}

Mesh::Mesh(glm::vec3 position, glm::vec2 size, int length)
    : m_Position(position), m_Size(size), m_Length(length)
{
    createMesh();
}

void Mesh::render()
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
}

void Mesh::createMesh()
{
     glm::vec3 sideIncrementX = {
        m_Size.x / (float)m_Length,
        0.0f,
        0.0f
    };

    glm::vec3 sideIncrementZ = {
        0.0f,
        0.0f,
        m_Size.y / (float)m_Length
    };

    glm::vec3 startPosition = glm::vec3 {
        m_Position.x - m_Size.x / 2,
        m_Position.y,
        m_Position.z - m_Size.y / 2
    };

    glm::vec3 currentPosition = startPosition;

    std::vector<Vertex> vertices;
    for (int z = 0; z < m_Length + 1; z++)
    {
        for (int x = 0; x < m_Length + 1; x++)
        {
            Vertex v;
            v.position = startPosition + sideIncrementX * (float)(x) + sideIncrementZ * (float)(z);
            v.texCoords = glm::vec2 { (float)x / (float)m_Length, (float)z / (float)m_Length };

            vertices.push_back(v);
        }
    }

    std::vector<uint32_t> indices;
    for (int z = 0; z < m_Length; z++)
    {
        for (int x = 0; x < m_Length; x++)
        {
            int tempLength = m_Length + 1;
            indices.emplace_back(z * tempLength + x);
            indices.emplace_back((z + 1) * tempLength + x);
            indices.emplace_back(z * tempLength + (x + 1));

            indices.emplace_back(z * tempLength + (x + 1));
            indices.emplace_back((z + 1) * tempLength + x);
            indices.emplace_back((z + 1) * tempLength + (x + 1));
        }
    }

    m_VertexCount = vertices.size();
    m_IndexCount = indices.size();

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);
}
