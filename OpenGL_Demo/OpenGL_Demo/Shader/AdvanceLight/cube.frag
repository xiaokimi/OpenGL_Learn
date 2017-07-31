#version 330 core

in VS_OUT 
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} fs_in;


out vec4 color;

uniform sampler2D texture1;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float LinearizeDepth(float depth)
{
	float near_plane = 0.0f;
	float far_plane = 0.0f;

	float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

float ShadowCalculation()
{
	vec4 fragPosLightSpace = fs_in.FragPosLightSpace;

	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;

	float currentDepth = projCoords.z;

	float bias = 0.005;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0f;

	return shadow;
}

void main()
{
	vec3 texColor = texture(texture1, fs_in.TexCoords).rgb;

	vec3 normal = normalize(fs_in.Normal);

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
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * texColor;

	color = vec4(lighting, 1.0f);
}