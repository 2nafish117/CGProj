#version 330 core

in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()
{
	vec4 texColor = texture(uTexture, vTexCoord);
	FragColor = texColor;
}