#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;

	vec3 TangentFragPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	
	vs_out.TexCoords = texCoords;
	vs_out.Normal = transpose(inverse(mat3(model))) * normal;
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
   	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
   	vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
   	mat3 TBN = transpose(mat3(T, B, N));

   	vs_out.TangentFragPos = TBN * vs_out.FragPos;
   	vs_out.TangentLightPos = TBN * lightPos;
   	vs_out.TangentViewPos = TBN * viewPos;
}

/*#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;   
    
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}*/