#include <glad/gl.h>
#include <fmt/core.h>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>

#include "board.hpp"

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

	u32 _program_id;
	u32 _vertex_array_id;
	u32 _vertex_buffer_id;
	u32 _texture_id;
	i32 _width;
	i32 _height;
	std::vector<Rgba> _pixels;
	std::vector<std::function<void()>> _render_callbacks;
} // namespace

namespace board {
	void init(u32 width, u32 height) {
		_width = (i32)width;
		_height = (i32)height;
		_pixels.resize(width * height, kWhite);

		// <Init shader program>
		_program_id = glCreateProgram();
		auto vert_id = load_shader(GL_VERTEX_SHADER, "fifnmar_cg_lab/board.vert");
		auto frag_id = load_shader(GL_FRAGMENT_SHADER, "fifnmar_cg_lab/board.frag");
		glAttachShader(_program_id, vert_id);
		glAttachShader(_program_id, frag_id);
		glLinkProgram(_program_id);
		check_err(_program_id, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);
		glDeleteShader(frag_id);
		glDeleteShader(vert_id);

		// <Vertex>
		glCreateVertexArrays(1, &_vertex_array_id);
		glEnableVertexArrayAttrib(_vertex_array_id, 0);
		glVertexArrayAttribBinding(_vertex_array_id, 0, 0);
		glVertexArrayAttribFormat(_vertex_array_id, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glCreateBuffers(1, &_vertex_buffer_id);
		glNamedBufferData(_vertex_buffer_id, sizeof(kVertices), kVertices.data(), GL_DYNAMIC_DRAW);
		glVertexArrayVertexBuffer(_vertex_array_id, 0, _vertex_buffer_id, 0, sizeof(Vertex));

		// <Texture>
		glCreateTextures(GL_TEXTURE_2D, 1, &_texture_id);
		glTextureParameteri(_texture_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureStorage2D(_texture_id, 1, GL_RGBA8, _width, _height);
	}

	void on_render(std::function<void()> callback) {
		_render_callbacks.emplace_back(std::move(callback));
	}

	void render() {
		std::fill(_pixels.begin(), _pixels.end(), kWhite);
		for (auto& i: _render_callbacks) { i(); }
		glTextureSubImage2D(_texture_id, 0, 0, 0, ::_width, ::_height, GL_RGBA, GL_UNSIGNED_BYTE, _pixels.data());
		glUseProgram(_program_id);
		glBindTextureUnit(0, _texture_id);
		glUniform1i(glGetUniformLocation(_program_id, "tex"), 0);
		glBindVertexArray(_vertex_array_id);
		glDrawArrays(GL_TRIANGLES, 0, kVertices.size());
	}

	void set_pixel(u32 x, u32 y, Rgba color) {
		_pixels[y * ::_width + x] = color;
	}

	u32 width() { return ::_width; }

	u32 height() { return ::_height; }
} // namespace board