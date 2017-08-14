#version 330 core

in VS_OUT 
{
	vec2 TexCoords;
} fs_in;


out vec4 color;

uniform sampler2D textureMap;
uniform sampler2D hdrMap;

uniform float exposure;
uniform bool useHdrMap;

uniform bool bloom;

void main()
{	
	const float gamma = 2.2f;
	vec3 hdrColor = texture(textureMap, fs_in.TexCoords).rgb;
	vec3 bloomColor = texture(hdrMap, fs_in.TexCoords).rgb;

	if (bloom)
		hdrColor += bloomColor;

	//vec3 mapped =  (hdrColor*(2.51*hdrColor+0.03))/(hdrColor*(2.43*hdrColor+0.59)+0.14);
	vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

	mapped = pow(mapped, vec3(1.0 / gamma));

	color = vec4(mapped, 1.0f);
}