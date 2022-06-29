#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Assert.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "data/CubeData.h"

const float VIEWPORT_HEIGHT  = 960.0f;
const float VIEWPORT_WIDTH   = 540.0f;
const float ASPECT_RATIO     = VIEWPORT_HEIGHT / VIEWPORT_WIDTH;

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // Run OpenGL 3.3.0 in 'Core' mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(VIEWPORT_HEIGHT, VIEWPORT_WIDTH, APP_NAME.c_str(), NULL, NULL);

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
		// Initialize GL
		GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glEnable(GL_CULL_FACE));

        // Bind vertex array buffer
        VertexArray va;
        VertexBuffer vb(CubePoints, sizeof(CubePoints));
        VertexBufferLayout layout;
        layout.Push<float>(3);
		layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(CubeIndices, NumberOfCubeIndices);

		// View Matrices
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), ASPECT_RATIO, 1.0f, 20.0f);
		glm::mat4 viewMatrix = glm::lookAt(
			glm::vec3(0.0f, 0.0f, -3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

		// Setup shader
		Shader shader("res/shaders/Flat.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 0.25f);

		// Setup texture
		Texture texture("res/textures/Crate.jpg");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

        // Clear glBuffers
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		// Runtime Data
		Renderer renderer;
		float rotAngle = 0.0f;
		std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();

        while (!glfwWindowShouldClose(window))
        {
			renderer.Clear();

			/* Update */

			// Update cube rotation
			auto currentTime = std::chrono::steady_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
			lastTime = currentTime;

			rotAngle += 0.001f * elapsedTime;
			if (rotAngle >= 360.0f)
				rotAngle = 0.0f;

			// Update view matrices
			glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;

			/* Draw */

			// Set Uniforms
			shader.Bind();
			shader.SetUniformMatrix4fv("u_ModelViewProjectionMatrix", glm::value_ptr(modelViewProjectionMatrix));

			renderer.Draw(va, ib, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
