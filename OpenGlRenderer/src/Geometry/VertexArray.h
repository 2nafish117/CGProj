#pragma once

#include <vector>

#include "OpenGLDependencies.h"

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:
	unsigned int mRendererID;

};