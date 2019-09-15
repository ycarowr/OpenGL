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
	float trianglePositions[]
	{
		-0.5f, -0.5f,
		0, 0.5f,
		0.5f, -0.5f
	};


	//All of it to only to define a single position of a triangle
	
	//define the size of the buffer (in bytes), check the documentation
	const int size = 6 * sizeof(float); // sizeof(trianglePositions);

	//the attribute we want to define is the positioning of a vertex of a triangle
	const int firstAttributeIndex = 0; //because is the first
	//enables the index
	glEnableVertexAttribArray(firstAttributeIndex);
	const int firstAttributeSizeInBytes = 2; //first for X and second for Y
	auto firstAttributeType = GL_FLOAT; //the type of our attribute
	const bool firstAttributeIsNormalized = GL_FALSE; //its already a float, don't need to be normalized

	//the stride works like an index. Its the offset between each element in bytes. In this case, two floats
	const int firstAttributeStride = sizeof(float) * 2;

	const int firstAttributePointer = 0; //because is the address of the first

	//define the layout of the data or how the gpu will read the data we 
	//provided inside the buffer (some ppl will call it shader)
	glVertexAttribPointer(
		firstAttributeIndex,
		firstAttributeSizeInBytes, 
		firstAttributeType, 
		firstAttributeIsNormalized, 
		firstAttributeStride, 
		firstAttributePointer);

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
