#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT 
{
	vec2 TexCoords;

	vec3 TangentFragPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	
	vs_out.TexCoords = texCoords;
	
	vec3 T = normalize(mat3(model) * tangent);
	vec3 B = normalize(mat3(model) * bitangent);
	vec3 N = normalize(mat3(model) * normal);
	mat3 TBN = transpose(mat3(T, B, N));

	vec3 FragPos = vec3(model * vec4(position, 1.0f));
	vs_out.TangentFragPos = TBN * FragPos;
	vs_out.TangentLightPos = TBN * lightPos;
	vs_out.TangentViewPos = TBN * viewPos;
}