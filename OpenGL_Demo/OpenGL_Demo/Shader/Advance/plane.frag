#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    color = texture(texture1, TexCoords);
    //color = vec4(vec3(gl_FragCoord.z), 1.0f);
}