#include "RenderColoredScreen.h"
#include "HelloTriangle.h"
#include "Application.h"

int main()
{
	const char* title = "LearnOpenGL";
	const int width = 800;
	const int height = 600;
	Application* app = new HelloTriangle();
	app->Initialize(title, width, height);
	app->RenderLoop();
	return 0;
}