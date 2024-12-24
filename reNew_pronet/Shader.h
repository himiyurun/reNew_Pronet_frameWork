#pragma once
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
	SHADER_BLOCK_SIZE,
	SHADER_BLOCK_INFO_END
};

class Shader
{
	GLuint program;

	uint8_t Block[SHADER_BLOCK_SIZE];

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

