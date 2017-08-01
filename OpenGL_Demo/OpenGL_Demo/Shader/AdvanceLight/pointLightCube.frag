#version 330 core

in VS_OUT 
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

out vec4 color;

uniform sampler2D texture1;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool inverse_normal;


float ShadowCalculation()
{
	vec3 fragToLight = fs_in.FragPos - lightPos;
	float closestDepth = texture(depthMap, fragToLight).r;

	closestDepth *= far_plane;
	float currentDepth = length(fragToLight);

	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

	return shadow;
}

void main()
{
	vec3 texColor = texture(texture1, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	if (inverse_normal)
	{
		normal *= -1.0f;
	}

	vec3 lightColor = vec3(1.0f);

	vec3 ambient = 0.15 * lightColor;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;

	float shadow = ShadowCalculation();
	//float shadow = 0.0f;
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * texColor;

	color = vec4(lighting, 1.0f);
}