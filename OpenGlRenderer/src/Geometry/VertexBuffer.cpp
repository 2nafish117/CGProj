#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	GLCall(glGenBuffers(1, &mRendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &mRendererID));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
}

void VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
