#pragma once

#include <GL/glew.h>
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h" 

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GlLog(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLog(const char* function, const char* file, int line);

class Renderer
{
private:

public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};