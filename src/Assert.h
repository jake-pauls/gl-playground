#ifndef Assert_h
#define Assert_h

#include <cstring>
#include <signal.h>

#include <glad/glad.h>

#define __SHORT_NAME__ strrchr("/" __FILE__, '/') + 1

#define LOG(x) \
    std::cout << "[THE_PLAY::" << __SHORT_NAME__ << "(" << __LINE__ << ")]" << x << std::endl;

// TODO: Include defines for compiler-specific debug breaks
#define ASSERT(x) if (!(x)) raise(SIGTRAP)

#define GL_CALL(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int lineNumber);

#endif /* Assert.h */
