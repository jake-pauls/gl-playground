#include "Renderer.hpp"

#include <iostream>

void GLClearErrors() 
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int lineNumber) 
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ":" << lineNumber << std::endl;
        return false;
    }

    return true;
}
