#version 330 core

in vec2 TexCoords;

uniform sampler2D ourTexture;

out vec4 color;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

uniform bool horizontal;

void main()
{
	vec2 tex_offset = 1.0 / textureSize(ourTexture, 0); // 获取一个纹理像素的大小

	vec3 result = texture(ourTexture, TexCoords).rgb * weight[0];

	if (horizontal)
	{
		for (int i = 1; i < 5; i++)
		{
			result += texture(ourTexture, TexCoords - vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
			result += texture(ourTexture, TexCoords + vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
		}
	}
	else
	{
		for (int i = 1; i < 5; i++)
		{
			result += texture(ourTexture, TexCoords + vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
			result += texture(ourTexture, TexCoords - vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
		}
	}

	color = vec4(result, 1.0f);
}