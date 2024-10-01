cmake_minimum_required(VERSION 3.5.0)
project(OpenGLEngine VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Add the executable
add_executable(OpenGLEngine 
    src/config.h
    src/main.cpp
    src/glad.c
)

# Include directories
target_include_directories(OpenGLEngine PRIVATE dependencies)

# Include libraries
target_link_libraries(OpenGLEngine 
    glfw
    OpenGL::GL
)