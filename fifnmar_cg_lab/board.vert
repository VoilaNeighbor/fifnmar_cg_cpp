#version 460 core
in layout(location = 0) vec2 position;

void main() {
	gl_Position = vec4(position, 0, 1);
}