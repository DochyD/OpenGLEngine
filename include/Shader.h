#pragma once

#include <string>

// Encapsulates creating, compiling, linking, and using an OpenGL shader program
class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    // Activate the shader program
    void use() const;

private:
    void checkCompileErrors(unsigned int shader, const std::string& type);
};