#version 330 core

in vec2 TexCoords;
in vec3 fColor;

out vec4 color;

uniform sampler2D texture1;

void main()
{
	//color = texture(texture1, TexCoords);
	color = vec4(fColor, 1.0f);
}