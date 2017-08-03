#version 330 core

in VS_OUT
{
	vec2 TexCoords;
	vec3 FragPos;
	vec3 Normal;

	mat3 TBN;
} fs_in;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	float shininess;
};

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;

uniform bool useNormalMap;

void main()
{
	vec4 diffuseColor = texture(material.texture_diffuse1, fs_in.TexCoords);
	vec4 ambient = 0.15f * diffuseColor;

	vec3 normal = normalize(fs_in.Normal);
	if (useNormalMap)
	{
		normal = texture(material.texture_normal1, fs_in.TexCoords).rgb;

		normal = normalize(normal * 2.0f - 1.0f);

		normal = normalize(fs_in.TBN * normal);
	}


	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec4 diffuse = diff * diffuseColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), material.shininess);
	vec4 specularColor = texture(material.texture_specular1, fs_in.TexCoords);
	vec4 specular = spec * specularColor;

	//color = ambient + diffuse + specular;
	color = texture(material.texture_normal1, fs_in.TexCoords);
}