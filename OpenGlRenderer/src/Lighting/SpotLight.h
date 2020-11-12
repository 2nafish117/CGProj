#pragma once

#include <glm/glm.hpp>

struct SpotLight
{
	glm::vec3 mPosition;
	glm::vec3 mDirection;
	float mInnerCutOff;
	float mOuterCutOff;

	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;

	SpotLight(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f),
		float inner = cos(glm::radians(35.0f)), float outer = cos(glm::radians(40.0f)),
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 diffuse = glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f))
		: mPosition(pos), mDirection(dir), 
		mInnerCutOff(inner), mOuterCutOff(outer),
		mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular) {  }

	~SpotLight() {  }

};
