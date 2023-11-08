#pragma once

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferElements.h"

class VertexArray 
{
private:
	unsigned int m_ArrayID;
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
	void AddAttr(const VertexBuffer& buffer, const VertexBufferElements& elements);
};