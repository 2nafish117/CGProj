#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Geometry.h"
#include "Material/Material.h"
#include "OpenGlDependencies.h"

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, 
		const std::vector<Texture>& textures);
	Mesh(const void* vertices, unsigned int size, const VertexBufferLayout& layout, 
		const unsigned int* indices, unsigned int count);
	~Mesh() {  }

	void addTexture(const Texture& texture);
	void draw(ShaderProgram& shader) const;
	void bind(ShaderProgram & shader) const;
	IndexBuffer getIbo() const;
public:
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	// cool way of indexing type of texture, but requires correct layout
	union {
		struct {
			// do not change order of these, or it wont work !!
			std::vector<Texture> mDiffuseTextures;
			std::vector<Texture> mSpecularMaps;
			// std::vector<Texture> mNormalMaps;
		};
		std::vector<Texture> mTextures[Texture::NumTextureTypes];
	};
private:
	void setupMesh();
private:
	VertexArray mVao;
	VertexBuffer mVbo;
	IndexBuffer mIbo;
};
