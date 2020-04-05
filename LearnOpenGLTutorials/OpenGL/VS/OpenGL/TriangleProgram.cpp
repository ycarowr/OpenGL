#pragma once
#include "TriangleProgram.h"
#include "Triangle.h"

void TriangleProgram::Initialize()
{
	const char* title = "LearnOpenGL";
	const int width = 800;
	const int height = 600;

	std::vector<float> vertices =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	Application app = Triangle(&vertices);
	app.Initialize(title, width, height, false);
	app.RenderLoop();
}