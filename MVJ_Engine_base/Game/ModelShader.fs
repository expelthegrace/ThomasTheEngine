#version 330 core

out vec4 color;

in vec2 uv0;
in vec3 normal0;

uniform sampler2D texture0;
uniform int drawTexture;
uniform vec4 color0;

void main()
{
    if (drawTexture == 1) color = vec4 (normal0.x, normal0.y, normal0.z, 1.0);// color = texture2D(texture0, uv0);
	else color = color0;
}
