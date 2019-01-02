#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv0;
layout(location = 2) in vec3 vertex_normal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 uv0;
out vec3 normal0;
out vec4 vertexPos;

void main()
{
	vertexPos = view * model * vec4(vertex_position, 1.0);
    gl_Position = proj * vertexPos;

    uv0 = vertex_uv0;

	//mat3 normalMatrix = inverse(transpose(mat3 (view * model)));
   // normal0 = normalize(vec3(normalMatrix * vertex_normal));
	normal0 = vertex_normal;
}
