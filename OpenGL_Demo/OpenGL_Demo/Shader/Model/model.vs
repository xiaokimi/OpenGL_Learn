#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
	vec2 TexCoords;
	vec3 FragPos;
	vec3 Normal;

	mat3 TBN;
} vs_out;


void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	
	vs_out.TexCoords = texCoords;
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(model))) * normal;

	vec3 T = normalize(mat3(model) * tangent);
	vec3 N = normalize(mat3(model) * normal);
	vec3 B = normalize(mat3(model) * bitangent);
	vs_out.TBN = mat3(T, B, N);
}