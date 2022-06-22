#ifndef Assert_h
#define Assert_h

#include <signal.h>

#include <glad/glad.h>

// TODO: Include defines for compiler-specific debug breaks
#define ASSERT(x) if (!(x)) raise(SIGTRAP)

#define GL_CALL(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int lineNumber);

#endif /* Assert.h */
