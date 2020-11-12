#include "Renderer.h"

#include "Geometry.h"

Renderer::Renderer()
{
	GLCall(const unsigned char* version = glGetString(GL_VERSION));
	std::cout << "OpenGl Version: " << version << std::endl;
}

void Renderer::draw(const VertexArray & va, const IndexBuffer & ib, const ShaderProgram & sp) const
{
	va.bind();
	ib.bind();
	sp.bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(const Mesh& mesh, ShaderProgram& shader) const
{
	shader.bind();
	mesh.bind(shader);
	GLCall(glDrawElements(GL_TRIANGLES, mesh.getIbo().getCount(), GL_UNSIGNED_INT, nullptr));
}

//void Renderer::draw(const Mesh& mesh, ShaderProgram& shader) const
//{
//	mesh.draw(shader);
//}

void Renderer::setClearColor(float red, float green, float blue, float alpha)
{
	GLCall(glClearColor(red, green, blue, alpha));
}

void Renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::enable(GLenum option)
{
	GLCall(glEnable(option));
}

void Renderer::setBlendFunc(GLenum sFactor, GLenum dFactor)
{
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::setCullingMode(GLenum face, GLenum winding)
{
	GLCall(glCullFace(face));
	GLCall(glFrontFace(winding));
}

