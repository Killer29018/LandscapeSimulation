#pragma once

#include "Window.hpp"
#include "Shader.hpp"
#include "EventHandler.hpp"
#include "Camera.hpp"

class Application : public EventSignaller, public EventObserver
{
public:
    Application(glm::ivec2 windowSize, const char* title);
    ~Application();

    void start();

    void attach(EventObserver* observer);
    void detach(EventObserver* observer);
private:
    std::list<EventObserver*> m_Observers;

    Window m_Window;
    uint32_t m_VAO;
    uint32_t m_VBO;

    Shader m_Shader;
    Camera m_Camera;
private:
    void initialize();
    void mainLoop();

    void notify(const Event* event);
};
