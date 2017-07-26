#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
    vec2 texCoords;
    vec3 normal;
} gs_in[];

out vec2 TexCoords;
out vec3 fColor;

uniform float time;

vec4 explore(vec4 position, vec3 normal)
{
    float magnitude = 2.0f;
    vec3 dis = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
    //vec3 dis = normal * magnitude;
    return position + vec4(dis, 0.0f);
    //return position;
}

void main()
{
	gl_Position = explore(gl_in[0].gl_Position, gs_in[0].normal); 
    //gl_Position = gl_in[0].gl_Position;
    TexCoords = gs_in[0].texCoords;
    fColor = vec3(1.0f, 0.0f, 0.0f);
    EmitVertex();
 
    gl_Position = explore(gl_in[1].gl_Position, gs_in[1].normal); 
    //gl_Position = gl_in[1].gl_Position;
    TexCoords = gs_in[1].texCoords;
    fColor = vec3(0.0f, 1.0f, 0.0f);
    EmitVertex();

    gl_Position = explore(gl_in[2].gl_Position, gs_in[2].normal); 
    //gl_Position = gl_in[2].gl_Position;
    TexCoords = gs_in[2].texCoords;
    fColor = vec3(0.0f, 0.0f, 1.0f);
    EmitVertex();

    EndPrimitive();
}