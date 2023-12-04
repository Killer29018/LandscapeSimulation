#pragma once

#include "Window.hpp"
#include "Shader.hpp"
#include "EventHandler.hpp"
#include "Camera.hpp"

#include "Mesh.hpp"

class Application : public EventSignaller, public EventObserver
{
public:
    Application(glm::ivec2 windowSize, const char* title);
    ~Application();

    void start();

    void keyboardEvent(const KeyboardEvent& event) override;
private:
    Window m_Window;

    Shader m_Shader;
    Camera m_Camera;

    Mesh m_Mesh;

    bool m_WireframeRender = false;
private:
    void initialize();
    void mainLoop();
};
