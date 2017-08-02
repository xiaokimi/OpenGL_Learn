#version 330 core

out vec4 color;

in VS_OUT
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
	mat3 TBN;
} fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture1;
uniform sampler2D normalMap;

uniform bool useNormalMap;

void main()
{
	vec3 texColor = texture(texture1, fs_in.TexCoords).rgb;

	vec3 lightColor = vec3(1.0f);

	vec3 ambient = 0.15f * lightColor;

	vec3 normal = normalize(fs_in.Normal);
	if (useNormalMap) 
	{
		normal = texture(normalMap, fs_in.TexCoords).rgb;

		normal = normalize(normal * 2.0f - 1.0f);

		normal = normalize(fs_in.TBN * normal);
	}

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;

	vec3 lighting = (ambient + diffuse + specular) * texColor;

	color = vec4(lighting, 1.0f);
}