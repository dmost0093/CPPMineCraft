#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/Camera.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Renderer.h"
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

glm::mat4 GetModelMatrix(float x, float y, float z)
{
	glm::mat4 World{ 1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					  x  ,  y  ,  z  , 1.0f };
	return World;
}

int main(void)
{
	Window mainWindow{ 1980, 1080, "Hello" };
	mainWindow.Initialize();


	Camera camera{ glm::vec3{0.0f, 0.0f, 5.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 5.0f, 0.5f };

	// Projection matrix
	float fovy = 3.14f / 3;
	float aspect = (float)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight();
	float near = 1.0f;
	float far = 100.f;
	glm::mat4 proj = glm::perspective(fovy, aspect, near, far);
	glm::mat4 model = GetModelMatrix(0.0f, 0.0f, 0.0f);
	glm::mat4 model1 = GetModelMatrix(2.0f, 0.0f, 0.0f);
	glm::mat4 view = camera.calculateViewMatrix();
	glm::mat4 mvp = proj * view * model;
	Renderer renderer(&camera);
	renderer.SetProjectionMat(proj);


	renderer.Prepare(RenderPass::PASS3D);

	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	const int numCubes = 5;
	std::vector<glm::mat4> cubePositions;
	
	for (int i = 0; i < numCubes; ++i)
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float increase = 2.0f;
		glm::mat4 temp = GetModelMatrix(x + increase * i, y, z);
		cubePositions.push_back(temp);
	}

	float r, g, b;
	r = 1.0f;
	g = 0.0f;
	b = 0.0f;

	
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
		renderer.Clear();
		
		view = camera.calculateViewMatrix();
		for (int i = 0; i < cubePositions.size(); i++) 
		{
			
			if (i % 3 == 0)
			{
				r = 1.0f;
				g = 0.0f;
				b = 0.0f;
			}
			else if (i % 3 == 1)
			{
				r = 0.0f;
				g = 1.0f;
				b = 0.0f;
			}
			else
			{
				r = 0.0f;
				g = 0.0f;
				b = 1.0f;
			}
			glm::vec4 color = { r,g,b,1.0f };
			bool colorChanged = false;
			model = cubePositions[i];

			renderer.RenderAABB(model, false, color);
			//renderer.Draw(VAO, IBO, shader);
			
			
		}
		
		
		renderer.RenderQuadColor(glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.5f, 0.8f, 1.0f), GetModelMatrix(0.0f, 4.0f, 0.0f));

		/* Swap front and back buffers */
		mainWindow.SwapBuffer();

		
	}

	glfwTerminate();
	return 0;
}