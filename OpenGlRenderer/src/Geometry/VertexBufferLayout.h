#pragma once

#include <vector>
#include <cassert>

#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type; // the datype of each of the component
	unsigned int count; // number of components per element (eg 3 x floats)
	unsigned char normalised;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLubyte);
		}

		assert(false && "unsupported data type");
		return 0;
	}
};

class VertexBufferLayout
{
public:

	VertexBufferLayout() : mStride(0) {  }

	template<typename T>
	void push(unsigned int count)
	{
		// dont use for types other than defined
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		mElements.push_back({ GL_FLOAT, count, GL_FALSE});
		mStride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		mElements.push_back({ GL_UNSIGNED_INT, count, GL_TRUE });
		mStride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		mStride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline unsigned int getStride() const { return mStride; }

	const std::vector<VertexBufferElement>& getElements() const { return mElements; }
private:
	unsigned int mStride;
	std::vector<VertexBufferElement> mElements;
};