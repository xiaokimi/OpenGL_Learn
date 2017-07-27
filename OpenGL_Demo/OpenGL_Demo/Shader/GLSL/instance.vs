#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texColor;
layout (location = 2) in vec3 offset;

out vec3 fColor;

uniform vec3 offsets[100];

void main()
{
	gl_Position = vec4(position + offset, 1.0f);
	fColor = texColor;
}