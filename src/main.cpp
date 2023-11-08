#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/Camera.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexBufferElements.h"
#include "graphics/Window.h"


glm::mat4 GetTranslationMatrix(float tx, float ty, float tz)
{
	glm::mat4 T{ 1.0f, 0.0f, 0.0f, tx,
				0.0f, 1.0f, 0.0f, ty,
				0.0f, 0.0f, 1.0f, tz,
				0.0f, 0.0f, 0.0f, 1.0f };
	return T;
}

glm::mat4 GetWorldMatrix()
{
	glm::mat4 World{ 1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f };
	return World;
}

int main(void)
{
	Window mainWindow{ 1980, 1080, "Hello" };
	mainWindow.Initialize();

	

	



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

	VAO.Bind();
	VBO.AddData(vertices, 24 * sizeof(float), false);
	VertexBufferElements elements;
	elements.Push<float>(3);
	VAO.AddAttr(VBO, elements);
	IBO.AddData(indice, 36 * sizeof(unsigned int), false);


	Camera camera{ glm::vec3{0.0f, 0.0f, 5.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 5.0f, 0.5f };

	// Projection matrix
	float fovy = 3.14f / 3;
	float aspect = (float)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight();
	float near = 1.0f;
	float far = 100.f;
	glm::mat4 proj = glm::perspective(fovy, aspect, near, far);
	glm::mat4 world = GetWorldMatrix();

	Shader shader{ "res/shaders/Basic.shader" };
	shader.Use();
	shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
	shader.SetUniformMat4f("u_ProjMat", proj);
	shader.SetUniformMat4f("u_ViewMat", camera.calculateViewMatrix());
	shader.SetUniformMat4f("u_WorldMat", world);

	VAO.Unbind();
	VBO.UnBind();
	IBO.UnBind();
	shader.UnUse();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	

	/* Loop until the user closes the window */
	while (!mainWindow.GetShouldClose())
	{
		float now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		shader.SetUniformMat4f("u_ViewMat", camera.calculateViewMatrix());

		shader.Use();
		VAO.Bind();
		IBO.Bind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		mainWindow.SwapBuffer();

		
	}

	glfwTerminate();
	return 0;
}