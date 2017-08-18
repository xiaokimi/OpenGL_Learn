#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 Normal;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

uniform vec3 viewPos;
uniform vec3 lightPositions[2];
uniform vec3 lightColors[2];

uniform bool useNormalMap;

vec3 CalcuLight(vec3 lightPos, vec3 lightColor, vec3 normal)
{
	float ambi = 0.1f;
	vec3 ambient = ambi * lightColor;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;

	float distance = length(fs_in.FragPos - lightPos);
    float attenuation = 1.0 / (distance * distance);

    diffuse *= attenuation;
    specular *= attenuation;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 texColor = texture(diffuseTexture, fs_in.TexCoords).rgb;

	vec3 normal = normalize(fs_in.Normal);
	if (useNormalMap)
	{
		normal = texture(normalTexture, fs_in.TexCoords).rgb;
		normal = normalize(normal * 2.0f - 1.0f);
	}

	vec3 lighting = vec3(0.0f);
	for (int i = 0; i < 2; i++)
	{
		lighting += CalcuLight(lightPositions[i], lightColors[i], normal);
	}

	vec3 result = lighting * texColor;

	float brightness = dot(result, vec3(0.2126f, 0.7152f, 0.0722f));
	//if (brightness > 1.0f)
		//BrightColor = vec4(result, 1.0f);

	FragColor = vec4(result, 1.0f);
	BrightColor = vec4(vec3(0.0f), 1.0f);
}