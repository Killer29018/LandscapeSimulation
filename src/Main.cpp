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

#include "Application.hpp"

const uint32_t SCREEN_WIDTH = 1000;
const uint32_t SCREEN_HEIGHT = 1000;

int main()
{
    Application* app = new Application { { SCREEN_WIDTH, SCREEN_HEIGHT }, "Landscape" };
    app->start();

    delete app;

    return 0;
}
