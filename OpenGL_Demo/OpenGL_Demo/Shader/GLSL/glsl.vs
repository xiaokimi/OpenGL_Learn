#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out VS_OUT
{
	vec2 texCoords;
	vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	vs_out.normal = vec3(projection * vec4(normalMatrix * normal, 1.0f));
	vs_out.texCoords = texCoords;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}