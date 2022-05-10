#include "board.hpp"
#include <glad/gl.h>
#include <fmt/core.h>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>

namespace {
	void check_err(u32 handle, auto status_t, auto get_iv, auto get_log) {
		i32 ok;
		get_iv(handle, status_t, &ok);
		if (ok) { return; }
		char err_info[1024];
		get_log(handle, sizeof(err_info), null, err_info);
		fmt::print(stderr, "<GlError>{}</GlError>\n", err_info);
		std::exit(-1);
	}

	u32 load_shader(auto shader_type, char const* shader_filename) {
		std::ifstream file(shader_filename);
		std::stringstream buf;
		buf << file.rdbuf();
		auto shader_content = buf.str();
		auto c_shader_content = shader_content.c_str();
		u32 id = glCreateShader(shader_type);
		glShaderSource(id, 1, &c_shader_content, null);
		glCompileShader(id);
		check_err(id, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);
		return id;
	}
} // namespace (helpers)

namespace {
	struct Vertex {
		f32 x, y;
	};

	std::array<Vertex, 6> kVertices {{
		{ -1, -1 },
		{ -1, 1 },
		{ 1, -1 },
		{ -1, 1 },
		{ 1, -1 },
		{ 1, 1 },
	}};

	u32 program_id;
	u32 vertex_array_id;
	u32 vertex_buffer_id;
	u32 texture_id;
	i32 width;
	i32 height;
	std::vector<Rgba> pixels;
} // namespace (states)

namespace board {
	void init(u32 width, u32 height) {
		::width = (i32)width;
		::height = (i32)height;
		pixels.resize(width * height, kWhite);

		// <Init shader program>
		program_id = glCreateProgram();
		auto vert_id = load_shader(GL_VERTEX_SHADER, "fifnmar_cg_lab/board.vert");
		auto frag_id = load_shader(GL_FRAGMENT_SHADER, "fifnmar_cg_lab/board.frag");
		glAttachShader(program_id, vert_id);
		glAttachShader(program_id, frag_id);
		glLinkProgram(program_id);
		check_err(program_id, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);
		glDeleteShader(frag_id);
		glDeleteShader(vert_id);

		// <Vertex>
		glCreateVertexArrays(1, &vertex_array_id);
		glEnableVertexArrayAttrib(vertex_array_id, 0);
		glVertexArrayAttribBinding(vertex_array_id, 0, 0);
		glVertexArrayAttribFormat(vertex_array_id, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glCreateBuffers(1, &vertex_buffer_id);
		glNamedBufferData(vertex_buffer_id, sizeof(kVertices), kVertices.data(), GL_DYNAMIC_DRAW);
		glVertexArrayVertexBuffer(vertex_array_id, 0, vertex_buffer_id, 0, sizeof(Vertex));

		// <Texture>
		glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
		glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureStorage2D(texture_id, 1, GL_RGBA8, ::width, ::height);
	}

	void render() {
		glTextureSubImage2D(texture_id, 0, 0, 0, ::width, ::height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
		glUseProgram(program_id);
		glBindTextureUnit(0, texture_id);
		glUniform1i(glGetUniformLocation(program_id, "tex"), 0);
		glBindVertexArray(vertex_array_id);
		glDrawArrays(GL_TRIANGLES, 0, kVertices.size());
	}

	void set_pixel(u32 x, u32 y, Rgba color) {
		pixels[y * ::width + x] = color;
	}

	u32 width() { return ::width; }
	u32 height() { return ::height; }
} // namespace board