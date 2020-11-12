#pragma once

#include <iostream>

#include "OpenGLDependencies.h"

# ifndef NDEBUG
	#define ASSERT(x) if (!(x)) __debugbreak();
	#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#endif

# ifdef NDEBUG
	#define GLCall(x) x
# endif

void GLClearError();
bool GLLogCall(const char* function, const char* fname, unsigned int line);