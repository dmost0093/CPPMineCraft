#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/IndexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexBufferElements.h"

int main(void)
{
	std::cout << "Hello World" << std::endl;

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1980, 1060, "640 x 480", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* glfwMakeContext() must be call after glewInit()*/
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR" << std::endl;
	}

	

	std::cout << glGetString(GL_VERSION) << std::endl; // print the GL_VERSION of my platform



	float min = -0.5f;
	float max = 0.5;


	float vertices[] = {
		min, min, min, // 0
		max, min, min, // 1
		max, max, min, // 2
		min, max, min, // 3

		min, min, max, // 4
		max, min, max, // 5
		max, max, max, // 6
		min, max, max  // 7

	};

	unsigned int indice[] = {
		1, 0, 3, 1, 3, 2, // north (-z)
		4, 5, 6, 4, 6, 7, // south (+z)
		5, 1, 2, 5, 2, 6, // east (+x)
		0, 4, 7, 0, 7, 3, // west (-x)
		2, 3, 7, 2, 7, 6, // top (+y)
		5, 4, 0, 5, 0, 1  // bottom (-y)
	};

	VertexArray VAO;
	VertexBuffer VBO;
	IndexBuffer IBO;

	/*Shader shader{ "res/shaders/Basic.shader" };
	shader.Use();
	glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
	shader.SetUniformVec4("u_Color", color);*/


	VAO.Bind();
	VBO.AddData(vertices, 24 * sizeof(float), false);
	VertexBufferElements elements;
	elements.Push<float>(3);
	VAO.AddAttr(VBO, elements);
	IBO.AddData(indice, 36 * sizeof(unsigned int), false);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}