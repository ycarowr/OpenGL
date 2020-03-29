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

void Update()
{
	DrawTriangleWithLegacyOGL();
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
