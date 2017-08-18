#version 330 core

in vec2 TexCoords;

uniform sampler2D ourTexture;
uniform sampler2D FragPosTexture;

out vec4 color;

uniform bool showHDR;

void main()
{
	const float gamma = 2.2f;
	vec3 hdrColor = texture(ourTexture, TexCoords).rgb;

	if (showHDR)
	{
		hdrColor += texture(FragPosTexture, TexCoords).rgb;
	}

	vec3 mapped = (hdrColor*(2.51*hdrColor+0.03))/(hdrColor*(2.43*hdrColor+0.59)+0.14);
    // Gamma矫正 
    mapped = pow(mapped, vec3(1.0 / gamma));

    //color = vec4(mapped, 1.0f);

    color = texture(ourTexture, TexCoords);
}