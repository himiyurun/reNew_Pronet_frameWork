#pragma once
#include <cstdarg>
#include <functional>
#include <glad/glad.h>

#include "readtxt.h"
#include "Uniform.h"

struct ShaderMakeInfo {
	std::string vsrc;
	std::string fsrc;
};

enum ShaderBlock {
	PNGL_WINDOW_PARAM,
	PN_GAME_OBJECT_PARAM,
	SHADER_BLOCK_SIZE,
	SHADER_BLOCK_INFO_END
};

static std::array<pronet::Uniform, SHADER_BLOCK_SIZE> unf{
	pronet::Uniform("window",sizeof(WindowParam)),
	pronet::Uniform("structure", sizeof(Structure2v_Param))
};

void getBlockBindInfo(GLuint program, GLuint buf[SHADER_BLOCK_SIZE]) {
	GLuint pointer(0);
	for (GLuint i = 0; i < SHADER_BLOCK_SIZE; i++) {
		if (unf[i].ub_binding(program, i)) {
			buf[pointer] = i;
			pointer++;
		}
	}
	if (pointer < SHADER_BLOCK_SIZE) { buf[pointer] = SHADER_BLOCK_INFO_END; }
}

void updateUniformObject(GLuint program, GLuint buf[SHADER_BLOCK_SIZE], const void* data, ...) {
	va_list args;
	va_start(args, data);
	const void* arg[SHADER_BLOCK_SIZE];
	GLuint uniform_index(0);
	//	‰Â•Ï’·ˆø”‚ð”z—ñ‚É•ÏŠ·
	while ((arg[uniform_index] = va_arg(args, const void*)) != nullptr) {
		uniform_index++;
	}
	va_end(args);

	for (int i = 0; i < SHADER_BLOCK_SIZE; i++) {
		if (buf[i] == SHADER_BLOCK_INFO_END) break;
		if (!unf[buf[i]].ub_binding(program, i))
			throw std::runtime_error("Shader is not found!! : updateUniformObject");
		unf[buf[i]].Update(arg[buf[i]]);
	}
}

class Shader
{
	GLuint program;

	GLuint Block[SHADER_BLOCK_SIZE];

public:
	Shader();

	~Shader();

	GLboolean init(const char* vsrc, const char* fsrc);

	void reset();

	GLboolean ProgramInfo(GLuint program) const;

	GLboolean ShaderInfo(GLuint shader, const char* name) const;

	void use() const { glUseProgram(program); }

	[[nodiscard]]GLuint getProgram() const { return program; }
};

