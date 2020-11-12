#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 lNormal;

uniform mat4 uModel;
uniform mat4 uViewProjection;

void main()
{
	gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0f);
	lNormal = aNormal;
}