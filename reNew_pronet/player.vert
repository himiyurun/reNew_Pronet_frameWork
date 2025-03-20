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
	bool intersect;
}ply;

vec2 scaling2v() {
	return vec2(win.scale / win.size);
}

vec4 scaling4v() {
	return vec4(vec2(win.scale / win.size), 1.0, 1.0);
}

mat2 rotatef(float _rad) {
	return mat2(cos(_rad), -sin(_rad), sin(_rad), cos(_rad));
}

void main()
{
	gl_Position = (position + vec4(ply.position, 0.0, 0.0)) * scaling4v();
}