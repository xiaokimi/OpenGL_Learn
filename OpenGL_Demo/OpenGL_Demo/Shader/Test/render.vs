#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out VS_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 Normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	vs_out.TexCoords = texCoords;
	vs_out.Normal = mat3(transpose(inverse(model))) * normal;
}