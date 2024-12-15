#version 150 core
in vec4 position;

layout(std140) uniform window {
	vec2 size;
	float scale;
	vec2 mousePos;
}win;

uniform float scale;
uniform vec2 size;
uniform vec2 location;

void main()
{
	gl_Position = position * vec4(scale / size, 1.0, 1.0) + vec4(location, 0.0, 0.0);
}