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

void Application::keyboardEvent(const KeyboardEvent& event)
{
    if (event.action != GLFW_PRESS) return;

    if (event.key == GLFW_KEY_RIGHT_CONTROL)
    {
        m_WireframeRender = !m_WireframeRender;
    }
    if (event.key == GLFW_KEY_R)
    {
        m_Mesh.regenerate();
    }
}

void Application::initialize()
{
    m_Camera = Camera { m_Window.getSize(), { 0.0f, 1.0f, 0.0f } };
    m_Window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_Shader.compileFromPath("res/shaders/basic.vert.glsl", "res/shaders/basic.frag.glsl");

    m_Window.attach(this);
    m_Window.attach(&m_Camera);
    attach(&m_Camera);
    attach(&m_Mesh);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    m_Mesh = Mesh { glm::vec3 { 0.0f }, glm::vec2 { 500.0f }, 100 };
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

        if (m_WireframeRender)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderEvent render;
        render.camera = &m_Camera;
        notify(&render);

        m_Window.swapBuffers();
    }
}
