#include "Renderer.h"

void Renderer::InitShaders()
{
	m_CurrentShader.shaderType = ShaderType::NONE;
	m_Shaders[static_cast<int>(ShaderType::CHUNK)].emplace_back(new Shader{ "res/shaders/Basic.shader" });
	m_Shaders[static_cast<int>(ShaderType::SKY)].emplace_back(new Shader{ "res/shaders/Basic.shader" });
	m_Shaders[static_cast<int>(ShaderType::BASIC_TEXTURE)].emplace_back(new Shader{ "res/shaders/Texture.shader" });
	m_Shaders[static_cast<int>(ShaderType::BASIC)].emplace_back(new Shader{ "res/shaders/Basic.shader" });
}

void Renderer::InitTextures()
{
	//TODO: add the textures
	return;
}

void Renderer::InitBuffers()
{
	m_VAO = new VertexArray();
	m_VBO = new VertexBuffer();
	m_IBO = new IndexBuffer();
}

Renderer::Renderer(Camera * camera) : m_Camera{camera}
{
	m_WireFrame = false;
	Init();
}

Renderer::~Renderer()
{
	for (Shader* shader : m_Shaders[0]) 
	{
		delete shader;
	}
	delete m_VAO;
	delete m_VBO;
	delete m_IBO;
}

void Renderer::Init()
{
	InitShaders();
	InitTextures();
	InitBuffers();
}



void Renderer::RenderQuadColor(const glm::vec2 size, const glm::vec4 color, const glm::mat4 model)
{
	UseShader(ShaderType::BASIC);
	glm::mat4 proj = m_Proj;// must be replace
	glm::mat4 view = m_Camera->calculateViewMatrix();
	glm::mat4 mvp = proj * view * model;
	m_CurrentShader.shaderPointer->SetUniformMat4f("u_MVP", mvp);
	m_CurrentShader.shaderPointer->SetUniformVec4f("u_Color", color);

	float halfWidth = size.x / 2.0f;
	float halfHeight = size.y / 2.0f;

	float data[] = {
		-halfWidth, -halfHeight, 0.0f, // 0
		-halfWidth,  halfHeight, 0.0f, // 1
		 halfWidth,  halfHeight, 0.0f, // 2
		 halfWidth, -halfHeight, 0.0f  // 3
	};

	unsigned int indices[] = {
		1, 0, 3,
		2, 1, 3
	};


	if (!m_VAO || !m_VBO || !m_IBO) {
		// Log an error if any of the buffers or the vertex array object is null
		std::cerr << "Error: Vertex array object or buffers are not initialized." << std::endl;
		return;
	}

	m_VAO->Bind();
	m_VBO->AddData(data, 4 * 3 * sizeof(float), false);
	VertexBufferElements elements;
	elements.Push<float>(3);
	m_VAO->AddAttr(*m_VBO, elements);
	m_IBO->AddData(indices, 6, false);

	if (!m_CurrentShader.shaderPointer) {
		// Log an error if the shader pointer is null
		std::cerr << "Error: Shader pointer is null." << std::endl;
		return;
	}

	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	Draw(*m_VAO, *m_IBO, *m_CurrentShader.shaderPointer);
}

void Renderer::RenderQuadTexture()
{
}

void Renderer::RenderAABB(glm::mat4 model, bool fillMode, glm::vec4 color)
{
	UseShader(ShaderType::BASIC);
	glm::mat4 proj = m_Proj;// must be replace
	glm::mat4 view = m_Camera->calculateViewMatrix();
	glm::mat4 mvp = proj * view * model;
	m_CurrentShader.shaderPointer->SetUniformMat4f("u_MVP", mvp);
	m_CurrentShader.shaderPointer->SetUniformVec4f("u_Color", color);

	
	// must be changed that related to aabb
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

	m_VAO->Bind();
	m_VBO->AddData(vertices, 24 * sizeof(float), false);
	VertexBufferElements elements;
	elements.Push<float>(3);
	m_VAO->AddAttr(*m_VBO, elements);
	m_IBO->AddData(indice, 36, false);

	Draw(*m_VAO, *m_IBO, *m_CurrentShader.shaderPointer);

}

void Renderer::UseShader(ShaderType shaderType)
{
	if (shaderType == m_CurrentShader.shaderType)
	{
		return;
	}
	m_CurrentShader.shaderType = shaderType;
	m_CurrentShader.shaderPointer = m_Shaders[static_cast<int>(shaderType)][0];
	m_CurrentShader.shaderPointer->Use();
}

void Renderer::Prepare(RenderPass pass) const
{
	switch (pass) 
	{
	case RenderPass::PASS2D:
		//TODO: set the camera as ortho
		glClear(GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case RenderPass::PASS3D:
		//glClearColor();// TODO: add the clear color in header file
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, m_WireFrame ? GL_LINE : GL_FILL);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}
}

void Renderer::Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader) const
{
	GLenum error;

	shader.Use();
	VAO.Bind();
	IBO.Bind();

	// Check for OpenGL errors after shader activation
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error after shader activation: " << error << std::endl;
	}

	// Check for OpenGL errors after buffer binding
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error after buffer binding: " << error << std::endl;
	}

	// Draw call
	glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr);

	// Check for OpenGL errors after draw call
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error after draw call: " << error << std::endl;
	}
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}