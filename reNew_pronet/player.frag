#version 330 core
out vec4 fragment;

layout(std140) uniform player {
	vec2 position;
	float rotate;
	uint motion_id;
	bool intersect;
}ply;

void main()
{
	if(ply.intersect) {
		fragment = vec4(0.0, 1.0, 0.0, 1.0);
	} else {
		fragment = vec4(1.0, 0.0, 0.0, 1.0);
	}
}