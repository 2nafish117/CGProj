#version 330 core

in vec3 lNormal;

out vec4 oColor;

uniform vec4 uTint;

void main()
{
	oColor = vec4(lNormal, 1.0f);
	//oColor = vec4(abs(lNormal), 1.0f);
	//oColor = vec4(-lNormal, 1.0f);
}