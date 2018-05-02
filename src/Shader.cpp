#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string & filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);

	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
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
		else if (type != ShaderType::NONE)
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}
//create a shader from the strings and attach it to the program
 unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);


	//shaders are now linked to program, dont't need them anymore
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

 unsigned int Shader::CompileShader(unsigned int type, const std::string source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		//shader did not compile succesfully
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght * sizeof(char));

		glGetShaderInfoLog(id, lenght, &lenght, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

 void Shader::SetUniform1i(const std::string & name, int value)
 {
	 GLCall(glUniform1i(GetUniformLocation(name), value));
 }

 void Shader::SetUniform1f(const std::string & name, float value)
 {
	 GLCall(glUniform1f(GetUniformLocation(name), value));
 }

 void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
 {
	 GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
 }

 int Shader::GetUniformLocation(const std::string & name)
 {
	 if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	 {
		 return m_UniformLocationCache[name];
	 }

	 
	 GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	 if (location == -1)
	 {
		 std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	 }

	 m_UniformLocationCache[name] = location;
	 
	 return location;
 }