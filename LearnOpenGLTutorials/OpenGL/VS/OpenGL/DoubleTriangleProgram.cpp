#include "DoubleTriangleProgram.h"
#include "Triangle.h"

void DoubleTriangleProgram::Initialize()
{
	const char* title = "LearnOpenGL";
	const int width = 800;
	const int height = 600;

	std::vector<float> vertices =
	{
		-0.8f, -0.4f, 0.0f,
		-0.4f, 0.4f, 0.0f,
		-0.0f,  -0.4f, 0.0f,

		0.8f, -0.4f, 0.0f,
		0.4f, 0.4f, 0.0f,
		0.0f,  -0.4f, 0.0f
	};

	Application app = Triangle(&vertices);
	app.Initialize(title, width, height, false);
	app.RenderLoop();
}