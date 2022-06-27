#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>

#include "Assert.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
        // Four points of a rectangle with two texture coordinates each
        float vertexPositions[] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f,   // 0
             0.5f, -0.5f, 1.0f, 0.0f,   // 1
             0.5f,  0.5f, 1.0f, 1.0f,   // 2
            -0.5f,  0.5f, 0.0f, 1.0f    // 3
        };

        // Rectangle indexes, reduce redundancy (drawing two triangles) by providing indexes to repeat during draw call
        unsigned int indices[] =
        {
            0, 1, 2,   // Bottom Left Triangle
            2, 3, 0,   // Top Right Triangle
        };

		// Initialize GL
		GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glEnable(GL_CULL_FACE));

        // Bind vertex array buffer
        unsigned int vertexArrayObject;

        VertexArray va;
        VertexBuffer vb(vertexPositions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
		layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Flat.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/Crate.jpg");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

        // Clear glBuffers
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

        float r = 0.0f;
        float colorIncrement = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
			renderer.Clear();

			renderer.Draw(va, ib, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
