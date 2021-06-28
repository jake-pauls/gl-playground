#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLExec(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/**
 * Error Handling
 */

static void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int lineNumber) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ") " << function << " -> " << " in " << file << " on line " << lineNumber << std::endl;
        return false;
    }

    return true;
}

/**
 * Shader Handling
 */

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string &fileHandle) {
    std::ifstream stream(fileHandle);

    // ShaderTypes act as index for stringstream to load shader content
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string &source) {
    unsigned int shaderId = glCreateShader(type);
    const char* src = source.c_str();

    GLExec(glShaderSource(shaderId, 1, &src, nullptr));
    GLExec(glCompileShader(shaderId));

    int ivResult;
    GLExec(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ivResult));

    if (ivResult == GL_FALSE) {
        int length;
        GLExec(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*)alloca(length * sizeof(char));
        GLExec(glGetShaderInfoLog(shaderId, length, &length, message));

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;

        GLExec(glDeleteShader(shaderId));
        return 0;
    }

    return shaderId;
}

static unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int glProgram = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach, Link, and Validate glProgram
    GLExec(glAttachShader(glProgram, vs));
    GLExec(glAttachShader(glProgram, fs));
    GLExec(glLinkProgram(glProgram));
    GLExec(glValidateProgram(glProgram));

    GLExec(glDeleteShader(vs));
    GLExec(glDeleteShader(fs));

    return glProgram;
}

/**
 * Main Loop
 */

int main(void) {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "jaytracer-core", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Failed to initialize glew after rendering OpenGL context." << std::endl;

    std::cout << "OpenGL v" << glGetString(GL_VERSION) << std::endl;

    // Four points of a rectangle
    float vertexPositions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    // Rectangle indexes, reduce redundancy (drawing two triangles) by providing indexes to repeat during draw call
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    // Initalize a buffer for the triangle and bind it to position data
    unsigned int vertexBuffer;
    GLExec(glGenBuffers(1, &vertexBuffer));
    GLExec(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLExec(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), vertexPositions, GL_STATIC_DRAW));

    unsigned int indexBuffer;
    GLExec(glGenBuffers(1, &indexBuffer));
    GLExec(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLExec(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    // Layout vertex buffer and attributes for OpenGL
    GLExec(glEnableVertexAttribArray(0));
    GLExec(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    ShaderProgramSource source = parseShader("res/shaders/Flat.shader");
    unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
    GLExec(glUseProgram(shader));

    while (!glfwWindowShouldClose(window)) {
        GLExec(glClear(GL_COLOR_BUFFER_BIT));

        GLExec(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    GLExec(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
