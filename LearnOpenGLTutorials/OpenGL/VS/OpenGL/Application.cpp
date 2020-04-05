#pragma once
#include "Application.h"
#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


Application::Application(std::vector<float>* vertices)
{
	Vertices = vertices;
}

Application::Application(std::vector<float>* vertices, std::vector<unsigned int>* indices)
{
	Vertices = vertices;
	Indices = indices;
}

void FrameBufferSizeCallback(GLFWwindow* m_Window, int width, int height)
{
	glViewport(0, 0, width, height);
}

const char* GetVertexShaderSource()
{
	return "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
}

const char* GetFragmentShaderSource()
{
	return "#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
}

unsigned int CreateShader(const char * vertexShaderSource, unsigned int type)
{
	unsigned int shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &vertexShaderSource, NULL);
	glCompileShader(shader);

	//error checking
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		auto typeName = type == GL_VERTEX_SHADER ? " Vertex " : " Fragment ";
		std::cout << "Error compilation shader: " << typeName << "\n" << infoLog << std::endl;
		return -1;
	}

	return shader;
}

unsigned int CreateShaderProgram(const unsigned int vertexId, const unsigned int fragmentId)
{
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexId);
	glAttachShader(shaderProgram, fragmentId);
	glLinkProgram(shaderProgram);

	//error checking
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error Creating Combined Shader Program\n" << infoLog << std::endl;
		return -1;
	}

	glUseProgram(shaderProgram);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
	return shaderProgram;
}

void Application::Initialize(const char* title, const int width, const int height, bool isWiredFrame)
{
	InitializeInternal(title, width, height);

	CreateVertexArrayObjects();
	CreateVertexBufferObjects();
	CreateElementArrayBuffer();
	const char* vertexShader = GetVertexShaderSource();
	const char* fragmentShader = GetFragmentShaderSource();
	unsigned int vertexShaderId = CreateShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fragmentShaderId = CreateShader(fragmentShader, GL_FRAGMENT_SHADER);
	ShaderProgram = CreateShaderProgram(vertexShaderId, fragmentShaderId);

	int indexOfVertexAttribute = 0;
	int sizeOfVertexAttribute = 3;
	auto typeOfData = GL_FLOAT;
	auto isNormalizedData = GL_FALSE;
	int stride = 3 * sizeof(float);
	auto weirdCallback = (void*)0;

	glVertexAttribPointer(
		indexOfVertexAttribute,
		sizeOfVertexAttribute,
		typeOfData,
		isNormalizedData, stride, weirdCallback);
	glEnableVertexAttribArray(0);

	if (isWiredFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::InitializeInternal(const char* title, const int width, const int height)
{
	//Initialize OpenGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Initialize Window
	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW m_Window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);

	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glfwSetFramebufferSizeCallback(m_Window, FrameBufferSizeCallback);
}

void Application::RenderLoop()
{
	OnStart();
	while (!glfwWindowShouldClose(m_Window))
	{
		OnRender();

		glUseProgram(ShaderProgram);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

bool Application::IsElementBond()
{
	std::vector<unsigned int> source = Indices[0];
	int size = sizeof(unsigned int) * Indices->size();
	return size > 0;
}

//Render without VAO
//void HelloTriangle::OnRender()
//{
//	// 0. copy our vertices array in a buffer for OpenGL to use
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
//	// 1. then set the vertex attributes pointers
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	// 2. use our shader program when we want to render an object
//	glUseProgram(ShaderProgram);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//}

void Application::OnRender()
{
	
}

void Application::OnStart()
{

}

void Application::CreateVertexBufferObjects()
{
	int sizeBuffer = 1;
	glGenBuffers(sizeBuffer, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::vector<float> source = Vertices[0];
	float* data = &source[0];
	int size = sizeof(float) * Vertices->size();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Application::CreateVertexArrayObjects()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::vector<float> source = Vertices[0];
	float* data = &source[0];
	int size = sizeof(float) * Vertices->size();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Application::CreateElementArrayBuffer()
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	std::vector<unsigned int> source = Indices[0];
	unsigned int* data = &source[0];
	int size = sizeof(unsigned int) * Indices->size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
