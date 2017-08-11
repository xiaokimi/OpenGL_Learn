#version 330 core

in VS_OUT 
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
} fs_in;


out vec4 color;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 viewPos;

uniform sampler2D textureMap;

vec3 calculight(vec3 lightPos, vec3 lightColor)
{
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;

	float distance = length(fs_in.FragPos - lightPos);
    float attenuation = 1.0 / (distance * distance);

	return (diffuse + specular) * attenuation;
}

void main()
{
	vec4 texColor = texture(textureMap, fs_in.TexCoords);

	float ambi = 0.15f;
	vec3 ambient = ambi * texColor.rgb;

	vec3 lighting = vec3(0.0f);
	for (int i = 0; i < 4; i++)
	{
		lighting += calculight(lightPositions[i], lightColors[i]);
	}

	color = vec4(ambient + lighting * texColor.rgb, 1.0f);
}