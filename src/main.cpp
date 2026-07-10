#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <iostream>

int main()
{
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    // Request an OpenGL 4.6 Core context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create an OpenGL window
    SDL_Window* window = SDL_CreateWindow(
        "OpenGLEngine",
        800,
        600,
        SDL_WINDOW_OPENGL
    );

    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // Create the OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load OpenGL functions
    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD\n";
        return 1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        glViewport(0, 0, 800, 600);
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}