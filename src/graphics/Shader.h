#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct ShaderProgramSource 
{
	std::string VertexSource;
	std::string FragSource;
};

class Shader 
{
private:
	std::string m_FilePath;
	unsigned int m_ShaderID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragShader);

	int GetUniformLocation(const std::string& name);
public:
	Shader(const std::string& filepath);
	~Shader();

	void Use() const;
	void UnUse() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUnifomr1f(const std::string& name, float value);
	void SetUnifrom3f(const std::string& name, float v0, float v1, float v2);
	void SetUnifomr4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformVec4(const std::string& name, const glm::vec4& vector);


};
