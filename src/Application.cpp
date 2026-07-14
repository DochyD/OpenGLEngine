#include "Application.h"
#include "Input.h"

#include <glad/glad.h>
#include <iostream>

Application::Application()
    : m_window("OpenGLEngine", SCR_WIDTH, SCR_HEIGHT)
{
}

Application::~Application() = default;

bool Application::init()
{
    if (!m_window.init())
        return false;

    // Load OpenGL functions
    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    if (!m_renderer.init())
        return false;

    return true;
}

void Application::run()
{
    bool running = true;
    while (running)
    {
        Input::processEvents(m_window, running);

        m_renderer.render();

        m_window.swapBuffers();
    }
}