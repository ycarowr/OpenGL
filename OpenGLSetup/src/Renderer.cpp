
#include "Renderer.h"
#include <iostream>

void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GlLog(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]: "
			<< error
			<< " function: " << function
			<< " file:" << file
			<< " line: " << line
			<< std::endl;
		return false;
	}
	return true;
}