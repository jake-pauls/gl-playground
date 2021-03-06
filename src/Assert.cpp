#include "Assert.h"

#include <iostream>

void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int lineNumber)
{
    while (GLenum error = glGetError()) {
        LOG("[OpenGL Error] (" << error << ") " << function << " " << file << ":" << lineNumber);
        return false;
    }

    return true;
}
