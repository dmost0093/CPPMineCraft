#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ArrayID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(0, &m_ArrayID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ArrayID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddAttr(const VertexBuffer& buffer, const VertexBufferElements& elements)
{
	Bind();
	buffer.Bind();

	const auto& t_Elements = elements.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < t_Elements.size(); i++) 
	{
		const auto& t_Element = t_Elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, t_Element.count, t_Element.type, t_Element.normalized, elements.GetStride(), (const void*)offset);
		offset += t_Element.count * VertexBufferElement::GetSizeOfType(t_Element.type);
	}
}
