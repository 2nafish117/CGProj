#pragma once

#include <glm/glm.hpp>

struct PointLight
{
	glm::vec3 mPosition;

	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;

	float mKConstant;
	float mKLinear;
	float mKQuadratic;

	PointLight(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 diffuse = glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f),
		float kConstant = 1.0f,
		float kLinear = 0.005f,
		float kQuadratic = 0.01f)
		: mPosition(pos),
		mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular),
		mKConstant(kConstant), mKLinear(kLinear), mKQuadratic(kQuadratic) {  }

	~PointLight() {  }

};
