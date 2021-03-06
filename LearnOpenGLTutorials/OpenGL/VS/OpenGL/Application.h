#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Application
{
public:
	Application(std::vector<float>* vertices);
	Application(std::vector<float>* vertices, std::vector<unsigned int>* indices);
	std::vector<float>* Vertices;
	std::vector<unsigned int>* Indices;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int ShaderProgram;
	bool IsWireFrame;
	
	void Initialize(const char* title, const int width, const int height, bool isWiredFrame);
	void RenderLoop();
protected:
	virtual void OnRender();
	virtual void OnStart();
private:
	void CreateVertexBufferObjects();
	void CreateVertexArrayObjects();
	void CreateElementArrayBuffer();
	void InitializeInternal(const char* title, const int width, const int height);
	GLFWwindow *m_Window;
};

