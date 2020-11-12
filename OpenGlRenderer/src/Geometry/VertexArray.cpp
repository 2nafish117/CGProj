#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &mRendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &mRendererID));
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(mRendererID));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), (void*)offset));
		GLCall(glEnableVertexAttribArray(i));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}

}
