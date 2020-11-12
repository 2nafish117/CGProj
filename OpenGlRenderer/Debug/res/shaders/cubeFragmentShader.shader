#version 330 core

in vec2 TextureCoord;
in vec3 Normal;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D uTexture;
uniform vec3 uCameraPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material uMaterial;
uniform Light uLight;

void main()
{
	vec4 texColor = texture(uTexture, TextureCoord);

	vec3 lightDir = normalize(uLight.position - fragPos);
	vec3 norm = normalize(Normal);

	vec3 ambient = uLight.ambient * uMaterial.ambient;

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = uLight.diffuse * (diff * uMaterial.diffuse);

	vec3 viewDir = normalize(uCameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * (spec * uMaterial.specular);

	fragColor = texColor * vec4(ambient + diffuse + specular, 1.0);
}