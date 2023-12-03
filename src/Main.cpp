#include <iostream>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <stdint.h>

#include <glm/gtx/string_cast.hpp>

#include <fstream>

#include "Shader.hpp"
#include "Window.hpp"
#include "EventHandler.hpp"
#include "Camera.hpp"

const uint32_t SCREEN_WIDTH = 1000;
const uint32_t SCREEN_HEIGHT = 1000;

class TemporaryClass : public EventSignaller
{
public:
    TemporaryClass() = default;
    ~TemporaryClass() = default;

    void attach(EventObserver* obs) override
    {
        m_Oberservers.push_back(obs);
    }

    void detach(EventObserver* obs) override
    {
        // Do Nothing
    }

    void update(float dt)
    {
        UpdateEvent event;
        event.dt = dt;

        auto it = m_Oberservers.begin();
        while (it != m_Oberservers.end())
        {
            (*it)->update(event);
            it++;
        }
    }
private:
    std::list<EventObserver*> m_Oberservers;
};

int main()
{
    TemporaryClass t;

    Window::setWindowHint(GLFW_RESIZABLE, GL_FALSE);
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Landscape");

    window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    float vertices[] = {
        //  Position        Colour
        -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -1.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    uint32_t VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader;
    shader.compileFromPath("res/shaders/basic.vert.glsl", "res/shaders/basic.frag.glsl");

    Camera camera { window.getSize(), { 0.f, 0.f, 0.f } };
    window.attach(&camera);
    t.attach(&camera);

    glm::mat4 model { 1.0f };

    float previousTime = 0.0f;
    float currentTime;

    while (window.isActive())
    {
        currentTime = glfwGetTime();
        float dt = currentTime - previousTime;
        previousTime = currentTime;

        t.update(dt);

        window.getEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();
        shader.setMat4("u_Model", model);
        shader.setMat4("u_View", camera.getViewMatrix());
        shader.setMat4("u_Projection", camera.getProjectionMatrix());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window.swapBuffers();
    }

    return 0;
}
