#version 330 core

struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float kConstant;
	float kLinear;
	float kQuadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 lNormal;
in vec3 lFragPos;

out vec4 oColor;

uniform vec4 uTint;

uniform vec3 uCameraPos;
uniform Material uMaterial;
uniform DirLight uDirLight;

#define NumPtLight 2
uniform PointLight uPointLight[NumPtLight];

#define NumSptLight 1
uniform SpotLight uSpotLight[NumSptLight];

vec3 CalcDirLight(DirLight light, vec3 lNormal, vec3 viewDir)
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 lightDir = normalize(-light.direction);

	vec3 diffuseTexture = uMaterial.diffuse;

	vec3 specularTexture = uMaterial.specular;

	ambient = light.ambient * diffuseTexture;

	float diff = max(dot(lightDir, lNormal), 0.0);
	diffuse = diff * light.diffuse * diffuseTexture;

	vec3 reflectDir = reflect(lightDir, lNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	specular = light.specular * (spec * specularTexture);

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 lNormal, vec3 viewDir)
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 diffuseTexture = uMaterial.diffuse;

	vec3 specularTexture = uMaterial.specular;

	vec3 fragToLightDir = normalize(light.position - lFragPos);

	ambient = light.ambient * diffuseTexture;

	float diff = max(dot(lNormal, fragToLightDir), 0.0);
	diffuse = light.diffuse * (diff * diffuseTexture);

	vec3 reflectDir = reflect(fragToLightDir, lNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	specular = light.specular * (spec * specularTexture);

	float dist = length(light.position - lFragPos);
	float attenuation = 1.0 / (light.kConstant + light.kLinear * dist + light.kQuadratic * dist * dist);

	// return ambient + attenuation * (diffuse + specular);
	return attenuation * (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 lNormal, vec3 viewDir)
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 diffuseTexture = uMaterial.diffuse;

	vec3 specularTexture = uMaterial.specular;

	vec3 lightDir = normalize(-light.direction);
	vec3 fragToLightDir = normalize(light.position - lFragPos);

	ambient = light.ambient * diffuseTexture;

	float cos_angle = dot(fragToLightDir, lightDir);
	if (cos_angle >= light.outerCutOff)
	{
		// light it up
		float epsilon = light.innerCutOff - light.outerCutOff;
		float intensity = min((cos_angle - light.outerCutOff) / epsilon, 1.0);
		float diff = max(dot(lNormal, lightDir), 0.0);
		diffuse = light.diffuse * (diff * diffuseTexture);

		vec3 viewDir = normalize(uCameraPos - lFragPos);
		vec3 reflectDir = reflect(-fragToLightDir, lNormal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
		specular = light.specular * (spec * specularTexture);
		diffuse *= intensity;
		specular *= intensity;
	}

	return ambient + diffuse + specular;
}

vec3 CalcPhongLighting(vec3 lNormal, vec3 viewDir)
{
	vec3 result = CalcDirLight(uDirLight, lNormal, viewDir);
	
	for(int i = 0;i < NumPtLight;++i)
		result += CalcPointLight(uPointLight[i], lNormal, viewDir);
	
	for (int i = 0; i < NumSptLight; ++i)
		result += CalcSpotLight(uSpotLight[i], lNormal, viewDir);

	return result;
}

void main()
{
	vec3 viewDir = normalize(lFragPos - uCameraPos);
	vec3 lNormal = normalize(lNormal);

	vec3 lighting = CalcPhongLighting(lNormal, viewDir);

	oColor = vec4(lighting, 1.0);
}