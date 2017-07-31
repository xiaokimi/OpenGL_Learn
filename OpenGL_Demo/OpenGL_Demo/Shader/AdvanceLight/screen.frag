#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D texture1;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * 1.0 - 7.5) / (7.5 + 1.0 - z * (7.5 - 1.0));
}

void main()
{
    float depthValue = texture(texture1, TexCoords).r;
    color = vec4(vec3(depthValue), 1.0f);
}