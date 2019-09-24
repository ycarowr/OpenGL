//An OpenGL documentation: http://docs.gl/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"


int location;
float incrementR = 0.05f;
float r = 0;
unsigned int shader;

void ChangeIncrement()
{
	if (r > 1.0f)
		incrementR = -0.05f;
	else if(r < 0.0f)
		incrementR = 0.05f;

	r += incrementR;
}

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	bool glewInitialization = glewInit();
	if (glewInitialization != GLEW_OK)
		std::cout << "Error, GLEW not Ok!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	
	{
		//----------------- Create Buffers 

		float positions[] //the data of the buffer
		{
			-0.5f, -0.5f, //0
			0.5, -0.5f,	  //1
			0.5f, 0.5f,   //2
			-0.5, 0.5f,   //3
		};

		unsigned int indices[] //data of the indexes
		{
			0, 1, 2,
			2, 3, 0
		};

		// ---------------- Vertex Buffer
		VertexBuffer vb = VertexBuffer(positions, 4 * 2 * sizeof(float));

		// ---------------- Vertex Buffer Layout
		VertexBufferLayout layout;
		layout.Push<float>(2);

		// ---------------- Vertex Array
		VertexArray va;
		va.AddBuffer(vb, layout);

		//----------------- Index Buffer

		IndexBuffer ib = IndexBuffer(indices, 6);

		//----------------- Shader

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		{
			//----------------- Uniform
			shader.SetUniforms4f("u_Color", 1.0f, 0.3f, 0.8f, 1.0f);
		}

		// ---------------- Renderer

		Renderer renderer;

		//-------------- Frame by frame
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			shader.Bind();
			shader.SetUniforms4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			
			//draw call
			renderer.Draw(va, ib, shader);

			ChangeIncrement();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}
