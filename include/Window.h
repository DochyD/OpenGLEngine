#pragma once

#include <SDL3/SDL.h>

// Owns the SDL window and its associated OpenGL context
class Window
{
public:
    Window(const char* title, int width, int height);
    ~Window();

    // Creates the SDL window and GL context. Returns false on failure.
    bool init();

    // Swaps the front/back buffers
    void swapBuffers();

    // Updates the stored size and the GL viewport to match (call on resize events)
    void onResize(int width, int height);

    SDL_Window* getSDLWindow() const { return m_window; }
    SDL_GLContext getContext() const { return m_context; }

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    const char* m_title;
    int m_width;
    int m_height;

    SDL_Window*   m_window  = nullptr;
    SDL_GLContext m_context = nullptr;
};