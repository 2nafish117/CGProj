#pragma once

#include "OpenGLDependencies.h"
#include "GLError.h"

class VertexBuffer
{
private:
	unsigned int mRendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();
	void bind() const;
	void unbind() const;
};