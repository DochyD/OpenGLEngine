cmake_minimum_required(VERSION 3.20)

project(OpenGLEngine LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)

# Find SDL source code and execute it's CMakeLists.txt
add_subdirectory(external/SDL3-3.4.12)

# Find it on disk as it's installed by default on windows
find_package(OpenGL REQUIRED)

# Create a 'glad' library from the glac.c file
add_library(glad STATIC
    external/glad/src/glad.c
)

# Tells the compiler where the 'glad' header files are
# GLAD will be compiled into the executable
target_include_directories(glad PUBLIC
    external/glad/include
)


# Creates an executable from those source libraries
add_executable(OpenGLEngine
    src/main.cpp
    src/Shader.cpp
    src/stb_image.cpp
)

# Add include folder
target_include_directories(OpenGLEngine PRIVATE
    include
    external/stb/include
    external/glm    
)


# Tell the executable to use those libraries
# Private means only OpenGLEngine needs those libraries
target_link_libraries(OpenGLEngine PRIVATE
    SDL3::SDL3
    OpenGL::GL
    glad
)

# Copy SDL3.dll next to the executable if it isn't already there or if it has changed.
add_custom_command(TARGET OpenGLEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:OpenGLEngine>
)
