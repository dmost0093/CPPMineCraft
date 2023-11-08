#ifdef DEBUG
#include <iostream>
#endif


#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); // Create the shader object
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id); // Use id to compile the shader


	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); // From shader program bring the result of the compilation
	if (result == GL_FALSE) // Case faile to compile
	{
#ifdef DEBUG
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* log = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, log);
		std::cout << "[ERROR]: Compilation is failed for " << (type == GL_VERTEX_SHADER ? "Vertex Shader." : "Fragement shader.") <<
			std::endl; // Print the message that which shader
		std::cout << log << std::endl;
		glDeleteShader(id); // Delete the shader

#endif
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragShader)
{
	// function to create the shader program, compile them, and link

	unsigned int program = glCreateProgram(); // create the shader program object
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

	// add the compiled shaders to the program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	// Link the shaders with program
	glLinkProgram(program);
	glValidateProgram(program);

	// Once the shader program is created we don't need indivisual shaders(vertex and fragment)
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
#ifdef DEBUG
		std::cout << "Warning: uniform '" << name << "' doesn't exist!\n";
		std::cout << location << std::endl;
#endif
	}

	m_UniformLocationCache[name] = location;

	return location;
}

Shader::Shader(const std::string& filepath) : m_FilePath{ filepath }, m_ShaderID{ 0 }
{
	ShaderProgramSource source = ParseShader(m_FilePath);
	m_ShaderID = CreateShader(source.VertexSource, source.FragSource);

}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderID);
}

void Shader::Use() const
{
	glUseProgram(m_ShaderID);
}

void Shader::UnUse() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUnifomr1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUnifrom3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUnifomr4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniformVec4(const std::string& name, const glm::vec4& vector)
{
	glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}
