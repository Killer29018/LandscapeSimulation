#include <iostream>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <stdint.h>

#include <fstream>

#include "Shader.hpp"
#include "Window.hpp"
#include "EventHandler.hpp"

const uint32_t SCREEN_WIDTH = 500;
const uint32_t SCREEN_HEIGHT = 500;

int main()
{
    Window::setWindowHint(GLFW_RESIZABLE, GL_FALSE);
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Landscape");

    glEnable(GL_DEPTH_TEST);
    float vertices[] = {
        //  Position   Colour
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f
    };

    uint32_t VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader;
    shader.compileFromPath("res/shaders/basic.vert.glsl", "res/shaders/basic.frag.glsl");

    while (window.isActive())
    {
        window.getEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window.swapBuffers();
    }

    return 0;
}
