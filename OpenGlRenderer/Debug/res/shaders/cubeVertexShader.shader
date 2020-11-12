#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoord;
layout(location = 2) in vec3 aNormal;

out vec2 TextureCoord;
out vec3 Normal;
out vec3 fragPos;

uniform mat4 uViewProjection;
uniform mat4 uModel;
uniform mat3 uNormal;

void main()
{
	gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
	fragPos = vec3(uModel * vec4(aPosition, 1.0));
	TextureCoord = aTextureCoord;
	Normal = uNormal * aNormal;
}