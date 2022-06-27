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
#include "Shader.h"

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
        LOG("[ERROR] Failed to initialize glad after rendering OpenGL context");

    LOG("[INFO] OpenGL v" << glGetString(GL_VERSION) << std::endl);
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

		Shader shader("res/shaders/Flat.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Clear glBuffers
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

        float r = 0.0f;
        float colorIncrement = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
            GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

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
    }

    glfwTerminate();
    return 0;
}
