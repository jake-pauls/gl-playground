#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int compileShader(unsigned int type, const std::string &source) {
    unsigned int shaderId = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int ivResult;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ivResult);

    if (ivResult == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

static unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int glProgram = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach, Link, and Validate glProgram
    glAttachShader(glProgram, vs);
    glAttachShader(glProgram, fs);
    glLinkProgram(glProgram);
    glValidateProgram(glProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return glProgram;
}

int main(void) {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Failed to initialize glew after rendering OpenGL context." << std::endl;

    std::cout << "OpenGL v" << glGetString(GL_VERSION) << std::endl;

    float trianglePositions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };

    // Initalize a buffer for the triangle and bind it to position data
    unsigned int triangleBuffer;
    glGenBuffers(1, &triangleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), trianglePositions, GL_STATIC_DRAW);

    // Layout vertext buffer and attributes for OpenGL
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // Sample vertex/fragment shaders to display a red triangle
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
