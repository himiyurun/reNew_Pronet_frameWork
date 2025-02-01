#version 330 core
in vec4 position;

layout(std140) uniform window{
	vec2 size;
	vec2 now_position;
	vec2 last_position;
	float scale;
}win;

uniform vec2 location;

vec2 convert_tr(vec2 _loc) {
	return vec2(_loc) * vec2(1.0, -1.0);
}

void main() {
	gl_Position = (position + vec4(convert_tr(location), 0.0, 0.0)) * vec4(win.scale / win.size, 1.0 ,1.0);
}