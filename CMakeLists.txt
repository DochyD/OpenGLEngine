cmake_minimum_required(VERSION 3.20)

project(OpenGLEngine LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(external/SDL3-3.4.12)

add_executable(OpenGLEngine
    src/main.cpp
)

target_link_libraries(OpenGLEngine PRIVATE
    SDL3::SDL3
)

add_custom_command(TARGET OpenGLEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE_DIR:OpenGLEngine>
)