#version 460 core
in layout(location = 0) vec2 position;
out vec2 uv;

void main() {
	gl_Position = vec4(position, 0, 1);
	uv = position / 2 + 0.5;
}