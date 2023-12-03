#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <optional>
#include <exception>
#include <list>

#include "EventHandler.hpp"

class window_existence_error : public std::exception
{
public:
    window_existence_error() {}
    const char* what() const noexcept { return "Window not created"; }
};

class Window : public EventSignaller
{
public:
    Window();
    Window(uint32_t screenWidth, uint32_t screenHeight, const char* title);

    ~Window();

    void getEvents();

    void attach(EventObserver* observer) override;
    void detach(EventObserver* observer) override;

    GLFWwindow* getWindow();
    const glm::ivec2& getSize();

    bool isActive() { return !glfwWindowShouldClose(getWindow()); }
    void swapBuffers() { glfwSwapBuffers(getWindow()); }

    static void setWindowHint(int32_t hint, int32_t value);

private:
    std::optional<GLFWwindow*> m_Window;
    glm::ivec2 m_Size;

    // Fast Write / Remove, Slow random access
    std::list<EventObserver*> m_Observers;
private:
    void initGLFW(const char* title);

    void notify(const Event* event);

    static void keyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseMoveEvent(GLFWwindow* window, double xPos, double yPos);
};
