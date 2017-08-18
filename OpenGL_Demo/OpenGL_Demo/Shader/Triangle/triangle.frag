#version 330 core

in vec2 TexCoords;

uniform sampler2D ourTexture;

out vec4 color;

void main()
{
	//color = texture(ourTexture, TexCoords);
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}