#pragma once
#include "Quad.h"
#include "QuadProgram.h"

std::vector<float> vertices = 
{
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};

std::vector<unsigned int> indices = 
{  
	// note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};


void QuadProgram::Initialize()
{
	const char* title = "LearnOpenGL";
	const int width = 800;
	const int height = 600;

	Application app = Quad(&vertices, &indices);
	app.Initialize(title, width, height, true);
	app.RenderLoop();
}