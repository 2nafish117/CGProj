#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord; 

out vec2 vTexCoord;

uniform mat4 uMVP;

void main()
{
	vTexCoord = texCoord;
	gl_Position = uMVP * vec4(position, 1.0);
}