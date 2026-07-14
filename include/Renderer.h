#pragma once

#include "Shader.h"

// Owns GPU resources (shader program, VAO/VBO/EBO) and issues draw calls
class Renderer
{
public:
    Renderer();
    ~Renderer();

    // Compiles shaders and sets up vertex buffers. Returns false on failure.
    bool init();

    // Clears the screen and draws the scene
    void render();

private:
    Shader* m_shader = nullptr;

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    void setupBuffers();
};