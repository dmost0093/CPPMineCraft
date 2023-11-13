#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	for (Shader* shader : m_Shaders[0]) 
	{
		delete shader;
	}
}

void Renderer::Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader) const
{
	shader.Use();
	VAO.Bind();
	IBO.Bind();
	glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}