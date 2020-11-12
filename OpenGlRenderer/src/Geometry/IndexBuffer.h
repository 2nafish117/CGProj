#pragma once

#include "OpenGLDependencies.h"
#include "GLerror.h"

class IndexBuffer
{
private:
	unsigned int mRendererID;
	unsigned int mCount;
public:
	IndexBuffer(const unsigned int* indices, unsigned int count);
	IndexBuffer();
	~IndexBuffer();
	void bind() const;
	void unbind() const;
	inline unsigned int getCount() const { return mCount; }
};