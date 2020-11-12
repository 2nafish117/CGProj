#pragma once

#include <iostream>

#include "GLError.h"
#include "OpenGLDependencies.h"

/*
zlibd.lib
zlibstatic.lib
assimp-vc141-mt.lib
*/
class VertexArray;
class IndexBuffer;
class ShaderProgram;
class Mesh;
class Material;

class Renderer
{
public:
	Renderer();
	void draw(const VertexArray & va, const IndexBuffer & ib, const ShaderProgram & sp) const;
	void draw(const Mesh& mesh, ShaderProgram& shader) const;
	void setClearColor(float red, float green, float blue, float alpha);
	void clear() const;
	void enable(GLenum option);
	void setBlendFunc(GLenum sFactor, GLenum dFactor);
	void setCullingMode(GLenum face, GLenum winding = GL_CCW);
};