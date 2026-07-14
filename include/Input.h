#pragma once

class Window;

// Handles polling and dispatching SDL events
class Input
{
public:
    // Polls all pending SDL events.
    // Sets running to false if a quit was requested.
    // Forwards resize events to the given Window so it can update its viewport.
    static void processEvents(Window& window, bool& running);
};