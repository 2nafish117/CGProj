#pragma once

#include <glm/glm.hpp>

struct DirLight
{
	glm::vec3 mDirection;

	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;

	DirLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3 diffuse = glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f))
		: mDirection(direction),
		mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular) {  }

	~DirLight() {  }

};
