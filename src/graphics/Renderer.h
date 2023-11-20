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
enum class RenderPass 
{
	PASS2D=0,
	PASS3D
};

struct CurrentShader
{
	ShaderType shaderType;
	Shader * shaderPointer;

};
class Renderer 
{
private:
	enum CameraType m_CameraType;
	Camera* m_Camera;

	std::vector<Shader *> m_Shaders[static_cast<int>(ShaderType::BASIC) + 1];
	CurrentShader m_CurrentShader;

	bool m_WireFrame;

	// buffers
	VertexArray* m_VAO;
	VertexBuffer* m_VBO;
	IndexBuffer* m_IBO;

	// Functions
	void InitShaders();
	void InitTextures();
	void InitBuffers();


	// must be remove
	glm::mat4 m_Proj;

public:
	Renderer(Camera * camera);
	~Renderer();
	void Init();
	void Prepare(enum RenderPass pass) const;
	void UseShader(enum ShaderType shaderType);
	void RenderQuadColor(const glm::vec2 size, const glm::vec4 color, const glm::mat4 model);
	void RenderQuadTexture();
	void RenderAABB(glm::mat4 model, bool fillMode, glm::vec4 color);
	void Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader) const;
	void Clear() const;

	// must be remove
	Shader* CurrentShader() { return m_CurrentShader.shaderPointer; }
	void SetProjectionMat(glm::mat4 proj) { m_Proj = proj; }
};