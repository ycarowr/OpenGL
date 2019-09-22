#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GlLog(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLog(const char* function, const char* file, int line);
