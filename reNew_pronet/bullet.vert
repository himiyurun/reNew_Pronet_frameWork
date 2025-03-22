#version 330 core
in vec4 position;

layout(std140) uniform window {
	vec2 size;
	vec2 nowPos;
	vec2 lastPos;
	float scale;
}win;

layout(std140) uniform bulletData {
	vec2 pos;
	float beg_rad;
	float radius;
}bulData;

layout(std140) uniform bullet {
	vec2 position;
	float rad;
}bul;

vec2 scaling2v() {
	return vec2(win.scale / win.size);
}

vec4 scaling4v() {
	return vec4(vec2(win.scale / win.size), 1.0, 1.0);
}

void scaleWindows() {
	gl_Position *= scaling4v();
}

vec4 rotate4v(vec4 _verts, float _deg) {
	float rad = radians(_deg);
	return vec4(_verts.x * cos(rad) - _verts.y * sin(rad)
			   ,_verts.x * sin(rad) + _verts.y * cos(rad)
			   ,_verts.z
			   ,_verts.w);
}

void main() {
	gl_Position = position + vec4(bulData.pos + bul.position, 0.0, 0.0);
	scaleWindows();
}