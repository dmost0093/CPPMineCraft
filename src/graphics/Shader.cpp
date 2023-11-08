#include "Shader.h"



Shader::Shader(const std::string& filepath)
	:m_FilePath{ filepath }, m_ShaderID{ 0 }
{
	ShaderProgramSource source = ParseShader(filepath);

	m_ShaderID = CreateShader(source.VertexSource, source.FragSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderID);
}

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
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Shader Compilation failed! " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id); // Delete the shader
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragShader)
{
	unsigned int program = glCreateProgram(); // Create the object for shader program
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// Once we create the shader file we don't need those
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::Use() const
{
	glUseProgram(m_ShaderID);
}

void Shader::UnUse() const
{
	glUseProgram(0);
}

// Binding the shader(**MUST bind the shader before working with uniform**)
void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), //v means matrix
		1, // Number of the matrix
		GL_FALSE, // Doing transfore?
		&matrix[0][0]); // Address of the first element in the matrix
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning: unform '" << name << "' doesn't exist!\n";
	}

	m_UniformLocationCache[name] = location;

	return location;
}