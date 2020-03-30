#include "RenderColoredScreen.h"
#include "Triangle.h"

const char* title = "LearnOpenGL";
const int width = 800;
const int height = 600;

std::vector<float> vertices =
{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

int main()
{
	Application app = Triangle(&vertices);
	app.Initialize(title, width, height);
	app.RenderLoop();
	return 0;
}