#version 330 core

out vec4 color;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform sampler2D texture1;

uniform bool blinn;

void main()
{
	vec3 texColor = texture(texture1, TexCoords).rgb;

	float ambientStrength = 0.05f;
	vec3 ambient = ambientStrength * texColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * texColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	
	float spec = 0.0f;
	if (blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir); 
		float spec = pow(max(dot(halfwayDir, norm), 0.0), 32);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	}
	
	vec3 specular = vec3(0.3) * spec;

	color = vec4(ambient + diffuse + specular, 1.0f);
}