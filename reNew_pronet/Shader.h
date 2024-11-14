#pragma once
#include <functional>

#include <glad/glad.h>

#include "readtxt.h"

class Shader
{
	GLuint program;

public:
	Shader(const char* vsrc, const char* fsrc);

	~Shader();

	GLboolean ProgramInfo(GLuint program) const;

	GLboolean ShaderInfo(GLuint shader, const char* name) const;

	GLuint getProgram() const { return program; }
};

