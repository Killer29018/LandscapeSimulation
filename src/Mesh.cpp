#include "Mesh.hpp"
#include "glad/gl.h"

#include <vector>

#include "Camera.hpp"

#include "FastNoise/FastNoise.h"

Mesh::Mesh()
    : m_Position(0.0f), m_Size(0.0f), m_Length(0)
{

}

Mesh::Mesh(glm::vec3 position, glm::vec2 size, int triangleSide)
    : m_Position(position), m_Size(size), m_Length(triangleSide + 1)
{
    generateData();
}

void Mesh::render(const RenderEvent& event)
{
    const Camera& camera = *event.camera;
    m_Shader.bind();
    m_Shader.setMat4("u_Model", glm::mat4(1.0f));
    m_Shader.setMat4("u_View", camera.getViewMatrix());
    m_Shader.setMat4("u_Projection", camera.getProjectionMatrix());

    m_Shader.setVec3("u_CameraView", camera.getFront());
    m_Shader.setVec3("u_CameraPosition", camera.getPosition());
    m_Shader.setVec2("u_HeightTextureSize", m_HeightTextureSize);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_HeightTexture);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Mesh::generateData()
{
    m_Shader.compileFromPath("res/shaders/mesh.vert.glsl", "res/shaders/mesh.frag.glsl");

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenTextures(1, &m_HeightTexture);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    generateMesh();
    generateHeight();
}

void Mesh::generateMesh()
{
    int triangles = m_Length - 1;

    glm::vec3 sideIncrementX = {
        m_Size.x / (float)(triangles),
        0.0f,
        0.0f
    };

    glm::vec3 sideIncrementZ = {
        0.0f,
        0.0f,
        m_Size.y / (float)(triangles)
    };

    glm::vec3 startPosition = glm::vec3 {
        m_Position.x - m_Size.x / 2,
            m_Position.y,
            m_Position.z - m_Size.y / 2
    };

    glm::vec3 currentPosition = startPosition;

    std::vector<Vertex> vertices;
    for (int z = 0; z < m_Length; z++)
    {
        for (int x = 0; x < m_Length; x++)
        {
            Vertex v;
            v.position = startPosition + sideIncrementX * (float)(x) + sideIncrementZ * (float)(z);
            v.texCoords = glm::vec2 { (float)x / (float)triangles, (float)z / (float)triangles };

            vertices.push_back(v);
        }
    }

    std::vector<uint32_t> indices;
    for (int z = 0; z < m_Length - 1; z++)
    {
        for (int x = 0; x < m_Length - 1; x++)
        {
            indices.emplace_back(z * m_Length + x);
            indices.emplace_back((z + 1) * m_Length + x);
            indices.emplace_back(z * m_Length + (x + 1));

            indices.emplace_back(z * m_Length + (x + 1));
            indices.emplace_back((z + 1) * m_Length + x);
            indices.emplace_back((z + 1) * m_Length + (x + 1));
        }
    }

    m_VertexCount = vertices.size();
    m_IndexCount = indices.size();
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::generateHeight()
{
    auto fractal = FastNoise::New<FastNoise::FractalFBm>();
    auto noise = FastNoise::New<FastNoise::Perlin>();

    fractal->SetSource(noise);
    fractal->SetGain(0.5);
    fractal->SetOctaveCount(3);

    auto node = fractal;

    const int size = 1024;
    std::vector<float> noiseOutput(size * size);
    m_HeightTextureSize = glm::vec2(size, size);

    node->GenUniformGrid2D(noiseOutput.data(), 0, 0, size, size, 0.01f, rand());

    glBindTexture(GL_TEXTURE_2D, m_HeightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size, size, 0, GL_RED, GL_FLOAT, noiseOutput.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}
