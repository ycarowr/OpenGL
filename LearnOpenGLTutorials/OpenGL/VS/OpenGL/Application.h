#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Application
{
public:
	Application(std::vector<float>* vertices);
	std::vector<float>* Vertices;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int ShaderProgram;
	
	void Initialize(const char* title, const int width, const int height);
	void RenderLoop();
protected:
	virtual void OnRender();
	virtual void OnStart();
private:
	void CreateVertexBufferObjects();
	void CreateVertexArrayObjects();
	void InitializeInternal(const char* title, const int width, const int height);
	GLFWwindow *m_Window;
};

