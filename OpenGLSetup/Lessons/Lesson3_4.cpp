//An OpenGL documentation: http://docs.gl/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

void DrawTriangleWithLegacyOGL()
{
	//a triangle
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();
}

void Start()
{
	//declaring how many buffers we want
	const int amount = 1;

	//declare the buffer, its basically an id or the 
	//address where the buffer is stored in memory
	unsigned int buffer;

	//send the buffer to opengl in order it to allocate the buffer properly
	glGenBuffers(amount, &buffer);

	//tells the opengl how we are gonna use the buffer, this case, as a simple array
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//declare the data we want to put in the buffer
	float trianglePositions [] 
	{
		-0.5f, -0.5f, 
		0, 0.5f, 
		0.5f, -0.5f
	};

	//define the size of the buffer (in bytes), check the documentation
	const int size = 6 * sizeof(float); // sizeof(trianglePositions);

	//the usage is a hint to opengl of how we are gonna use the it, check documentation
	auto usage = GL_STATIC_DRAW;

	//finally send the data 
	glBufferData(GL_ARRAY_BUFFER, size, trianglePositions, usage);
}

void Update()
{
	//type of stuff we wanna draw
	auto type = GL_TRIANGLES;

	//we want to draw from the beginning
	const int begin = 0;

	//all the way up to the end of the array
	const int count = 3;
	
	//a draw call!
	glDrawArrays(type, begin, count);
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	bool glewInitialization = glewInit();
	if (glewInitialization != GLEW_OK)
		std::cout << "Error, GLEW not Ok!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	//before loop
	Start();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		Update();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}
