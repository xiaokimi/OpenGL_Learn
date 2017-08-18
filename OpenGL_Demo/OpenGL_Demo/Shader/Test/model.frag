#version 330 core

in VS_OUT 
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 Normal;
} fs_in;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	float shininess;
};

out vec4 color;

uniform Material material;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
	vec3 diffuseColor = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
	vec3 specularColor = texture(material.texture_specular1, fs_in.TexCoords).rgb;

	vec3 lightColor = vec3(1.0f);

	float ambi = 0.1f;
	vec3 ambient = ambi * lightColor * diffuseColor;

	vec3 normal = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor * diffuseColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), material.shininess);
	vec3 specular = spec * lightColor * specularColor;

	vec3 result = ambient + diffuse + specular; 

	color = vec4(result, 1.0f);
	//color = vec4(diffuseColor, 1.0f);
}