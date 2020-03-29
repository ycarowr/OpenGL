#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <chrono>
#include <thread> 

GLFWwindow* window;


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int GetRandom()
{
	return rand();
}

void InitializeOpenGL()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void InitializeWindow()
{
	const char* title = "LearnOpenGL";
	const int width = 800;
	const int height = 600;
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
}

void InitializeGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

void InitializeCallbacks()
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

//Loop
void OnRender()
{
	int isEven = GetRandom() % 2;
	std::cout << "Random " << isEven << std::endl;
	if (isEven == 1)
		glClearColor(1, 0, 0, 1.0f);
	else
		glClearColor(0, 1, 0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void RenderLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		OnRender();
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main()
{
	InitializeOpenGL();
	InitializeWindow();
	InitializeGlad();
	InitializeCallbacks();
	RenderLoop();
	return 0;
}
