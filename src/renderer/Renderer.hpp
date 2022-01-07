#ifndef Renderer_h
#define Renderer_h

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLExec(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int lineNumber);

#endif /* Renderer.h */
