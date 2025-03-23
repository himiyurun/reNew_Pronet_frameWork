#version 330 core
in vec2 tv;
out vec4 fragment;

uniform sampler2D tex;

void main() {
	fragment = texture(tex, tv);
}