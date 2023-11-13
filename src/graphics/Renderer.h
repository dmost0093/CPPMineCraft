#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferElements.h"
#include "Window.h"

#define SHADERS_LAST SHADER_BASIC
enum class ShaderType
{
	NONE = 0,
	CHUNK,
	SKY,
	BASIC_TEXTURE,
	BASIC

};
class Renderer 
{
private:
	enum CameraType m_CameraType;

	std::vector<Shader *> m_Shaders[static_cast<int>(ShaderType::BASIC) + 1];
	ShaderType m_CurrentShader;

public:
	Renderer();
	~Renderer();
	void Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader) const;
	void Clear() const;
};