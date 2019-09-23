#pragma once
#include <unordered_map>
#include <string>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	void SetUniforms4f(const std::string& name, float v0, float v1, float f2, float f3);

private:
	bool CompileShader();
	int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string source);
	ShaderProgramSource ParseShader(const std::string& filePath);
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

