#pragma once
#include <GL/glew.h>

class IndexBuffer 
{
private:
	unsigned int m_BufferID;
public:
	IndexBuffer();
	~IndexBuffer();
	void Bind() const;
	void UnBind() const;
	void AddData(const unsigned int* data, unsigned int size, bool dynamic);
};