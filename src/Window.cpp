#include "Window.h"

#include <glad/glad.h>
#include <iostream>

Window::Window(const char* title, int width, int height)
    : m_title(title), m_width(width), m_height(height)
{
}

Window::~Window()
{
    if (m_context) SDL_GL_DestroyContext(m_context);
    if (m_window) SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Window::init()
{
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return false;
    }

    // Request an OpenGL 4.6 Core context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create an SDL/OpenGL window
    m_window = SDL_CreateWindow(
        m_title,
        m_width,
        m_height,
        SDL_WINDOW_OPENGL
    );

    if (!m_window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return false;
    }

    // Create the OpenGL context
    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context)
    {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        SDL_Quit();
        return false;
    }

    return true;
}

void Window::swapBuffers()
{
    SDL_GL_SwapWindow(m_window);
}

void Window::onResize(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}