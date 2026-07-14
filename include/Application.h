#pragma once

#include "Window.h"
#include "Renderer.h"

// Ties together the Window, Input, and Renderer and drives the main loop
class Application
{
public:
    Application();
    ~Application();

    // Initializes the window, GL context, GLAD, and renderer. Returns false on failure.
    bool init();

    // Runs the main loop until the window is closed
    void run();

private:
    static const int SCR_WIDTH = 800;
    static const int SCR_HEIGHT = 600;

    Window   m_window;
    Renderer m_renderer;
};