#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application
{
public:
	void Initialize(const char* title, const int width = 800, const int height = 600);
	void RenderLoop();

protected:
	GLFWwindow *window;
	virtual void OnRender();
	virtual void OnStart();

private:
	void InitializeOpenGL();
	void InitializeWindow(const char* title, const int width = 800, const int height = 600);
	void InitializeGlad();
	void InitializeCallbacks();
};

