#include "Texture.h"

Texture::Texture(const std::string & path, Texture::Type type, int mode)
	: mRendererID(0), mType(type)
{

	GLCall(glGenTextures(1, &mRendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));

	stbi_set_flip_vertically_on_load(1);
	int width, height;
	unsigned char* buff = stbi_load(path.c_str(), &width, &height, nullptr, 4);
	if (buff == nullptr)
	{
		std::cerr << "ERROR::TEXTURE: cant load " << path << std::endl;
	}
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff));
	GLCall(glGenerateTextureMipmap(mRendererID));

	// GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	
	if (buff)
		stbi_image_free(buff);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &mRendererID));
}

void Texture::bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}