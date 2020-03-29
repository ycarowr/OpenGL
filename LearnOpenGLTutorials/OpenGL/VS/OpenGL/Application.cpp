#include "Application.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

void Application::Initialize(const char* title, const int width, const int height)
{
	InitializeOpenGL();
	InitializeWindow(title, width, height);
	InitializeGlad();
	InitializeCallbacks();
	
	RenderLoop();
}

void Application::InitializeOpenGL()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::InitializeWindow(const char* title, const int width, const int height)
{
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
}

void Application::InitializeGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Application::InitializeCallbacks()
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void OnRender()
{

}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Application::RenderLoop()
{
	OnStart();
	while (!glfwWindowShouldClose(window))
	{
		OnRender();
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::OnRender()
{
}

void Application::OnStart()
{
}

