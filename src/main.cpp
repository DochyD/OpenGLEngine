#include <SDL3/SDL.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <Camera.h>
#include <Shader.h>
#include <stb_image.h>

// Forward declaration
void processEvents(SDL_Window *window, bool &running);
void processInput(SDL_Window *window, bool &running);

//
namespace GLConfig {
constexpr int kVersionMajor = 4;
constexpr int kVersionMinor = 6;
constexpr int kDepthSize = 24;
} // namespace GLConfig

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0F, 0.0F, 3.0F));

// Timing
float deltaTime = 0.0F; // time between current frame and last frame
float lastFrame = 0.0F;

int main() {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    // Request an OpenGL 4.6 Core context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GLConfig::kVersionMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GLConfig::kVersionMinor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, GLConfig::kDepthSize);

    // Create an SDL/OpenGL window
    SDL_Window *window = SDL_CreateWindow("OpenGLEngine", SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);

    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowRelativeMouseMode(window, true);

    // Create the OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load OpenGL functions
    if (gladLoadGL() == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        return 1;
    }
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("shaders/vertex/color.vs", "shaders/fragment/color.fs");
    Shader lightCubeShader("shaders/vertex/light_cube.vs", "shaders/fragment/light_cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO;
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the
    // light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it;
    // the VBO's data already contains all we need (it's already bound, but we do it again for
    // educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    bool running = true;
    while (running) {

        // udpate time
        float currentFrame = SDL_GetTicks() / 1000.0F;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processEvents(window, running);
        processInput(window, running);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        SDL_GL_SwapWindow(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void processEvents(SDL_Window *window, bool &running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            running = false;
            break;

        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
            int width;
            int height;
            SDL_GetWindowSizeInPixels(window, &width, &height);
            glViewport(0, 0, width, height);
            break;
        }

        case SDL_EVENT_MOUSE_MOTION: {
            float xoffset = event.motion.xrel;
            float yoffset =
                -event.motion.yrel; // reversed since y-coordinates go from bottom to top
            camera.ProcessMouseMovement(xoffset, yoffset);
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            camera.ProcessMouseScroll(event.wheel.y);
            break;
        }
        }
    }
}

void processInput(SDL_Window *window, bool &running) {
    const bool *state = SDL_GetKeyboardState(nullptr);

    // Global
    if (state[SDL_SCANCODE_ESCAPE]) {
        running = false;
    }

    // Camera
    if (state[SDL_SCANCODE_W]) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (state[SDL_SCANCODE_S]) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (state[SDL_SCANCODE_A]) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (state[SDL_SCANCODE_D]) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}