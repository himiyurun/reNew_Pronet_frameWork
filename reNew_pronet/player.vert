#version 330 core
in vec4 position;

layout(std140) uniform window {
	vec2 size;
	vec2 nowPos;
	vec2 lastPos;
	float scale;
}win;

layout(std140) uniform player {
	vec2 position;
	float rotate;
	uint motion_id;
}ply;

uniform vec2 location;

vec2 scaling() {
	return vec2(win.scale / win.size);
}

void main()
{
	gl_Position = (position + vec4(ply.position, 0.0, 0.0)) * vec4(scaling(), 1.0, 1.0);
}