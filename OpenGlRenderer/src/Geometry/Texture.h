#pragma once

#include <string>

#include "OpenGLDependencies.h"
#include "GLError.h"
#include "stb_image/stb_image.h"

class Texture
{
public:
	enum Type {
		Diffuse,
		Specular,
		//Normal,
		NumTextureTypes
	};

	Type mType;

public:
	Texture(const std::string & path, Type type = Texture::Diffuse, int mode = GL_CLAMP_TO_BORDER);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

private:
	unsigned int mRendererID;

};