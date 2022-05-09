#version 460 core
uniform sampler2D tex;
in vec2 uv;
out vec4 result;

void main() {
	result = texture(tex, uv);
}