#include "Material.h"
#include <cassert>

void Material::bind() const
{
	mShader.bind();
}

void Material::unbind() const
{
	mShader.unbind();
}
