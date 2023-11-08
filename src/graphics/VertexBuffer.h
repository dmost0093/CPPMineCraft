#pragma once
#include <GL/glew.h>

class VertexBuffer
{
private:
	unsigned int m_BufferID;
public:
	VertexBuffer();
	~VertexBuffer();
	void Bind() const;
	void UnBind() const;
	void AddData(const float* data, unsigned int size, bool dynamic);

};