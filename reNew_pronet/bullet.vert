#version 330 core
#define OBJECT_ANGLE_VERTEX	layout(location = 0)
#define OBJECT_UV_VERTEX	layout(location = 1)
OBJECT_ANGLE_VERTEX in vec4 position;
OBJECT_UV_VERTEX in vec2 uv;

out vec2 tv;

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
	tv = uv;
	gl_Position = position + vec4(bulData.pos + bul.position, 0.0, 0.0);
	scaleWindows();
}