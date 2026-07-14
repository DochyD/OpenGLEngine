#include "Input.h"
#include "Window.h"

#include <SDL3/SDL.h>

void Input::processEvents(Window& window, bool& running)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // Quit event
            case SDL_EVENT_QUIT:
                running = false;
                break;

            // Resize event
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            {
                int w, h;
                SDL_GetWindowSizeInPixels(window.getSDLWindow(), &w, &h);
                window.onResize(w, h);
                break;
            }
        }
    }
}