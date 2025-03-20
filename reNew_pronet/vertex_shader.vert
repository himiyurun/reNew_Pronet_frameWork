#version 330 core
in vec4 position;

layout(std140) uniform window {
	vec2 size;
	vec2 nowPos;
	vec2 lastPos;
	float scale;
}win;

layout(std140) uniform structure {
	vec2 location;
	float rotate;
}str;

vec2 scaling2v() {
	return vec2(win.scale / win.size);
}

vec4 scaling4v() {
	return vec4(vec2(win.scale / win.size), 1.0, 1.0);
}

vec4 rotate4v(vec4 _verts, float _deg) {
	float rad = radians(_deg);
	return vec4(_verts.x * cos(rad) - _verts.y * sin(rad)
			   ,_verts.x * sin(rad) + _verts.y * cos(rad)
			   ,_verts.z
			   ,_verts.w);
}

mat2 rotate2m(float _rad) {
	return mat2(cos(_rad) , -sin(_rad)
	           ,sin(_rad) , cos(_rad));
}

mat4 rotate4m(float _rad) {
	return mat4( cos(_rad), -sin(_rad), 1.0, 1.0
			   , sin(_rad), cos(_rad), 1.0, 1.0
			   , 1.0, 1.0, 1.0, 1.0
			   , 1.0, 1.0, 1.0, 1.0);
}

void main()
{
	vec4 local = rotate4v(position, str.rotate);
	vec4 world = vec4(str.location, 0.0, 0.0);
	gl_Position =  (local + world) * scaling4v();
}