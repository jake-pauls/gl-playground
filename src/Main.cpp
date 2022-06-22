#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>

#include "Assert.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

/**
 * Shader Handling
 */

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& fileHandle)
{
    std::ifstream stream(fileHandle);

    // ShaderTypes act as index for stringstream to load shader content
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int shaderId = glCreateShader(type);
    const char* src = source.c_str();

    GL_CALL(glShaderSource(shaderId, 1, &src, nullptr));
    GL_CALL(glCompileShader(shaderId));

    int ivResult;
    GL_CALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ivResult));

    if (ivResult == GL_FALSE)
    {
        int length;
        GL_CALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*) malloc(length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(shaderId, length, &length, message));

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;

        GL_CALL(glDeleteShader(shaderId));
        return 0;
    }

    return shaderId;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int glProgram = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach, Link, and Validate glProgram
    GL_CALL(glAttachShader(glProgram, vs));
    GL_CALL(glAttachShader(glProgram, fs));
    GL_CALL(glLinkProgram(glProgram));
    GL_CALL(glValidateProgram(glProgram));

    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));

    return glProgram;
}

/**
 * Main Loop
 */

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // Run OpenGL 3.3.0 in 'Core' mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "jaytracer-core", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        std::cout << "Failed to initialize glad after rendering OpenGL context." << std::endl;

    std::cout << "OpenGL v" << glGetString(GL_VERSION) << std::endl;
    {
        // Four points of a rectangle
        float vertexPositions[] =
        {
            -0.5f,  0.5f,   // 0
            -0.5f, -0.5f,   // 1
             0.5f, -0.5f,   // 2
             0.5f,  0.5f,   // 3
        };

        // Rectangle indexes, reduce redundancy (drawing two triangles) by providing indexes to repeat during draw call
        unsigned int indices[] =
        {
            0, 1, 2,   // Bottom Left Triangle
            2, 3, 0,   // Top Right Triangle
        };

        // Bind vertex array buffer
        unsigned int vertexArrayObject;

        VertexArray va;
        VertexBuffer vb(vertexPositions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = parseShader("res/shaders/Flat.shader");
        unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
        GL_CALL(glUseProgram(shader));

        // Uniform example (rendered per draw)
        int uniformLocation = glGetUniformLocation(shader, "u_Color");
        ASSERT(uniformLocation != -1); // Shader was found on compilation

        // Clear glBuffers
        GL_CALL(glBindVertexArray(0));
        GL_CALL(glUseProgram(0));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float colorIncrement = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
            GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

            GL_CALL(glUseProgram(shader));
            GL_CALL(glUniform4f(uniformLocation, r, 0.3f, 0.8f, 1.0f));

            va.Bind();
            ib.Bind();

            GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            // Basic color variation logic
            if (r > 1.0f) colorIncrement = -0.05f;
            else if (r < 0.0f) colorIncrement = 0.05f;
            r += colorIncrement;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Cleanup
        GL_CALL(glDeleteProgram(shader));
    }

    glfwTerminate();
    return 0;
}
