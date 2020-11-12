#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoord;

out vec2 lTextureCoord;
out vec3 lNormal;
out vec3 lFragPos;

uniform mat4 uViewProjection;
uniform mat4 uModel;
uniform mat3 uNormal;

void main()
{
	gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
	lFragPos = vec3(uModel * vec4(aPosition, 1.0));
	lTextureCoord = aTextureCoord;
	lNormal = uNormal * aNormal;
}