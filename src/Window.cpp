#include "Window.hpp"
#include "EventHandler.hpp"
#include "GLFW/glfw3.h"

#include <iostream>
#include <algorithm>
#include <format>

Window::Window() {}

Window::Window(uint32_t screenWidth, uint32_t screenHeight, const char* title)
{
    m_Size = glm::ivec2 { screenWidth, screenHeight };

    initGLFW(title);

    attach(this);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::getEvents()
{
    glfwPollEvents();
}

void Window::attach(EventObserver* observer)
{
    m_Observers.push_back(observer);
}

void Window::detach(EventObserver* observer)
{
    auto position = std::find(m_Observers.begin(), m_Observers.end(), observer);
    m_Observers.erase(position);
}

GLFWwindow* Window::getWindow()
{
    if (m_Window)
        return m_Window.value();
    else
        throw window_existence_error();
}

const glm::ivec2& Window::getSize()
{
    return m_Size;
}

void Window::keyboardEvent(const KeyboardEvent& event)
{
    if (event.action != GLFW_PRESS) return;

    switch (event.key)
    {
    case GLFW_KEY_ESCAPE:
        shouldClose(true);
        break;
    }
}

void Window::mouseEnterEvent(const MouseEnterEvent& event)
{
    if (event.entered)
    {
        m_FirstMouse = true;
    }
}

void Window::setInputMode(int32_t mode, int32_t value)
{
    glfwSetInputMode(getWindow(), mode, value);
}

void Window::setWindowHint(int32_t hint, int32_t value)
{
    glfwWindowHint(hint, value);
}


void Window::initGLFW(const char* title)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(getSize().x, getSize().y, title, NULL, NULL);
    if (!getWindow())
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(getWindow());

    int version = gladLoadGL(glfwGetProcAddress);
    if (!version)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    std::cout << "OpenGL Version: " << GLAD_VERSION_MAJOR(version) << "." <<
        GLAD_VERSION_MINOR(version) << "\n";

    glViewport(0, 0, getSize().x, getSize().y);

    glfwSetWindowUserPointer(getWindow(), (void*)this);

    glfwSetKeyCallback(getWindow(), keyboardEvent);
    glfwSetCursorPosCallback(getWindow(), mouseMoveEvent);
    glfwSetCursorEnterCallback(getWindow(), mouseEnteredEvent);
}

void Window::notify(const Event* event)
{
    auto it = m_Observers.begin();

    while (it != m_Observers.end())
    {
        (*it)->receiveEvent(event);
        it++;
    }
}

void Window::keyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    KeyboardEvent event;
    event.key = key;
    event.action = scancode;
    event.action = action;
    event.mods = mods;

    w->notify(&event);
}

void Window::mouseMoveEvent(GLFWwindow* window, double xPos, double yPos)
{
    static float previousX = 0.0f;
    static float previousY = 0.0f;

    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    MousePositionEvent positionEvent;
    positionEvent.xPosition = xPos;
    positionEvent.yPosition = yPos;

    w->notify(&positionEvent);

    MouseMoveEvent moveEvent;
    if (w->m_FirstMouse)
    {
        previousX = xPos;
        previousY = yPos;
        w->m_FirstMouse = false;
    }
    moveEvent.xOffset = xPos - previousX;
    moveEvent.yOffset = previousY - yPos;

    previousX = xPos;
    previousY = yPos;

    w->notify(&moveEvent);
}

void Window::mouseEnteredEvent(GLFWwindow* window, int entered)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    MouseEnterEvent event;
    event.entered = entered;

    w->notify(&event);
}
