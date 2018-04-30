#include "VertexArray.h"
#include "Renderer.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_rendererID));
}


VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		/*
	glVertexAttribPointer

	index = 0 (position)
	size = how many floats represent this vertex attribute
	type = float
	Normalized = should the data be normalized , here: no
	stride = amount of bytes between each vertex
	pointer = how many bytes from start of vertex to get to an attribute
	here 0 because we're setting position

	Vertex array object and buffer are bound together here
	*/
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_rendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

