#include "GLError.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
		;
}

bool GLLogCall(const char* function, const char* fname, unsigned int line)
{
	bool isNoError = true;
	while (GLenum error = glGetError())
	{
		std::cerr << "[OpenGl Error] ( " << std::hex << error << std::dec << " ) FILE: " << fname << " LINE: " << line << "\n";
		std::cerr << "FUNCTION: " << function << "\n";
		isNoError = false;
	}
	return isNoError;
}