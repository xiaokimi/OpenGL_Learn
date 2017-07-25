#version 330 core

in VS_OUT
{
	vec2 TexCoords;
} fs_in;


uniform sampler2D frontTexture;
uniform sampler2D backTexture;

out vec4 color;

void main()
{
	if (gl_FrontFacing)
		color = texture(frontTexture, fs_in.TexCoords);
	else
		color = texture(backTexture, fs_in.TexCoords);
	//color = mix(texture(frontTexture, TexCoords), texture(backTexture, TexCoords), 0.2);
}