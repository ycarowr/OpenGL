#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
};

