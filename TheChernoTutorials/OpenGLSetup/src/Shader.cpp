#include "Shader.h"
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

Shader::Shader(const std::string& filePath)
	:m_FilePath(filePath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GlCall(glDeleteProgram(m_RendererID));
}
 
void Shader::Bind() const
{
	GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GlCall(glUseProgram(0));
}

void Shader::SetUniforms1i(const std::string & name, int i)
{
	GlCall(glUniform1i(GetUniformLocation(name), i));
}

void Shader::SetUniforms1f(const std::string & name, float v0)
{
	GlCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniforms4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GlCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	
	GlCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	m_UniformLocationCache[name] = location;
	return location;
}

bool Shader::CompileShader()
{
	return 0;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string source)
{
	unsigned int id;
	GlCall(id = glCreateShader(type));
	const char* src = source.c_str();
	const int amount = 1;
	auto length = nullptr;
	GlCall(glShaderSource(id, amount, &src, length));
	GlCall(glCompileShader(id));

	{
		int result;
		GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(sizeof(char) * length);
			GlCall(glGetShaderInfoLog(id, length, &length, message));
			std::cout
				<< "Shader Compilation Error: "
				<< (type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader: ")
				<< message << std::endl;

			GlCall(glDeleteShader(id));
			return 0;
		}
	}

	return id;
}


//creates a function to read the file at the path and create the shaders 
ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);
	std::string line;
	std::stringstream ss[2];
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType index = ShaderType::NONE;
	auto invalid = std::string::npos;
	while (getline(stream, line))
	{
		if (line.find("#shader") != invalid)
		{
			if (line.find("vertex") != invalid)
				index = ShaderType::VERTEX;
			else if (line.find("fragment") != invalid)
				index = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)index] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)//strings because it is the actual source code
{
	unsigned int program;
	GlCall(program = glCreateProgram());
	unsigned int vs;
	GlCall(vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	unsigned int fs;
	GlCall(fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));
	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));
	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));
	return program;
}