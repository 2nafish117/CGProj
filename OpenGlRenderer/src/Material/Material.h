#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ShaderProgram.h"
#include "Geometry/Texture.h"

class Material
{
public:
	Material(const ShaderProgram& shader) { mShader = shader; }
	Material() {  }
	~Material() {  }

	void bind() const;
	void unbind() const;

	void addTexture(const Texture & texture, Texture::Type type = Texture::Diffuse);
private:
	ShaderProgram mShader;
};