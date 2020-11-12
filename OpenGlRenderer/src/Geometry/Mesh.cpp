#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
	: mVertices(vertices), mIndices(indices)

{
	setupMesh();
	for (const auto& it : textures)
		addTexture(it);
}

Mesh::Mesh(const void * vertices, unsigned int size, const VertexBufferLayout & layout, const unsigned int * indices, unsigned int count)
{
	// Doesn't store vertex data into mVertices and mIndices
	mVao.bind();
	mVbo = VertexBuffer(vertices, size);
	mVao.addBuffer(mVbo, layout);
	mIbo = IndexBuffer(indices, count);
}

void Mesh::addTexture(const Texture& texture)
{
	// TODO: support for normal maps
	// cool way of indexing into texture type !!
	// assert("invalid texture type " && type < Texture::NumTextureTypes);
	auto& tex = mTextures[texture.mType];
	tex.push_back(texture);
}

void Mesh::draw(ShaderProgram & shader) const
{
	mVao.bind();
	mVbo.bind();
	mIbo.bind();
	shader.bind();
	// uMaterial.diffuse1
	// uMaterial.diffuse2
	// uMaterial.diffuse3
	// uMaterial.specular1
	// uMaterial.specular2

	int slot = 0;
	int diffuseNum = 0;
	for (const auto& it : mDiffuseTextures)
	{
		it.bind(slot);
		// TODO: maybe optimise here, use c style strings
		shader.seti("uMaterial.diffuse" + std::to_string(diffuseNum), slot);
		diffuseNum++;
		slot++;
	}
	int specularNum = 0;
	for (const auto& it : mSpecularMaps)
	{
		it.bind(slot);
		// TODO: maybe optimise here, use c style strings
		shader.seti("uMaterial.specular" + std::to_string(specularNum), slot);
		specularNum++;
		slot++;
	}

	GLCall(glDrawElements(GL_TRIANGLES, mIbo.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Mesh::bind(ShaderProgram & shader) const
{
	mVao.bind();
	mVbo.bind();
	mIbo.bind();
	shader.bind();
	// uMaterial.diffuse1
	// uMaterial.diffuse2
	// uMaterial.diffuse3
	// uMaterial.specular1
	// uMaterial.specular2

	int slot = 0;
	int diffuseNum = 0;
	for (const auto& it : mDiffuseTextures)
	{
		it.bind(slot);
		// TODO: maybe optimise here, use c style strings
		shader.seti("uMaterial.diffuse" + std::to_string(diffuseNum), slot);
		diffuseNum++;
		slot++;
	}
	int specularNum = 0;
	for (const auto& it : mSpecularMaps)
	{
		it.bind(slot);
		// TODO: maybe optimise here, use c style strings
		shader.seti("uMaterial.specular" + std::to_string(specularNum), slot);
		specularNum++;
		slot++;
	}
}

IndexBuffer Mesh::getIbo() const
{
	return mIbo;
}

void Mesh::setupMesh()
{
	mVao.bind();
	mVbo = VertexBuffer(mVertices.data(), mVertices.size() * sizeof(float));
	VertexBufferLayout layout;
	layout.push<float>(3);	// position
	layout.push<float>(3);	// normals
	layout.push<float>(2);	// texture coords
	mVao.addBuffer(mVbo, layout);
	mIbo = IndexBuffer(mIndices.data(), mIndices.size());
}