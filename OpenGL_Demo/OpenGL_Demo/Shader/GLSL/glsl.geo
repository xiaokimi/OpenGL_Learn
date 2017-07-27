#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT
{
    vec2 texCoords;
    vec3 normal;
} gs_in[];

out vec2 TexCoords;

uniform float time;

vec4 explore(vec4 position, vec3 normal)
{
    float magnitude = 2.0f;
    vec3 dis = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
    return position + vec4(dis, 0.0f);
}

void main()
{
	/*gl_Position = explore(gl_in[0].gl_Position, gs_in[0].normal); 
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
 
    gl_Position = explore(gl_in[1].gl_Position, gs_in[1].normal); 
    TexCoords = gs_in[1].texCoords;
    EmitVertex();

    gl_Position = explore(gl_in[2].gl_Position, gs_in[2].normal); 
    TexCoords = gs_in[2].texCoords;
    EmitVertex();*/

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].normal * 0.2, 0.0f);
    EmitVertex();

    EndPrimitive();
}