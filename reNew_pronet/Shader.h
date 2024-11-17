#pragma once
#include <functional>

#include <glad/glad.h>

#include "readtxt.h"

class Shader
{
	GLuint program;

public:
	Shader();

	~Shader();

	GLboolean Init(const char* vsrc, const char* fsrc);

	GLboolean ProgramInfo(GLuint program) const;

	GLboolean ShaderInfo(GLuint shader, const char* name) const;

	void use() const { glUseProgram(program); }

	[[nodiscard]]GLuint getProgram() const { return program; }
};

