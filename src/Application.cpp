#include "Application.hpp"
#include "EventHandler.hpp"
#include "GLFW/glfw3.h"
#include "glad/gl.h"

#include <algorithm>

Application::Application(glm::ivec2 windowSize, const char* title)
    : m_Window(Window(windowSize, title)) { }

Application::~Application() { }

void Application::start()
{
    initialize();

    mainLoop();
}

void Application::attach(EventObserver* observer)
{
    m_Observers.push_back(observer);
}

void Application::detach(EventObserver* observer)
{
    auto pos = std::find(m_Observers.begin(), m_Observers.end(), observer);
    m_Observers.erase(pos);
}

void Application::initialize()
{
    m_Camera = Camera { m_Window.getSize(), { 0.0f, 0.0f, 0.0f } };
    m_Window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_Shader.compileFromPath("res/shaders/basic.vert.glsl", "res/shaders/basic.frag.glsl");

    m_Window.attach(&m_Camera);
    attach(&m_Camera);

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

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Application::mainLoop()
{
    float previousTime = 0.0f;
    float currentTime;

    while (m_Window.isActive())
    {
        currentTime = glfwGetTime();
        float dt = currentTime - previousTime;
        previousTime = currentTime;

        UpdateEvent event;
        event.dt = dt;
        notify(&event);

        m_Window.getEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model { 1.0f };
        m_Shader.bind();
        m_Shader.setMat4("u_Model", model);
        m_Shader.setMat4("u_View", m_Camera.getViewMatrix());
        m_Shader.setMat4("u_Projection", m_Camera.getProjectionMatrix());

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        m_Window.swapBuffers();
    }
}

void Application::notify(const Event* event)
{
    for (auto it = m_Observers.begin(); it != m_Observers.end(); it++)
    {
        (*it)->receiveEvent(event);
    }
}
