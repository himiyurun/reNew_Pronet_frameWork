#pragma once
#include <glad/glad.h>

#include "readtxt.h"
#include "Uniform.h"

struct ShaderMakeInfo {
	std::string vsrc;
	std::string fsrc;
};

class Shader
{
	GLuint program;

	size_t Block[SHADER_BLOCK_SIZE];

	GLuint texUnitID;

public:
	Shader();

	~Shader();

	GLboolean init(const char* vsrc, const char* fsrc);

	void reset();

	GLboolean ProgramInfo(GLuint program) const;

	GLboolean ShaderInfo(GLuint shader, const char* name) const;

	void use() const { 
		glUseProgram(program);
		pronet::bindUniformObject(Block);
	}

	[[nodiscard]] GLuint getProgram() const { return program; }
	[[nodiscard]] GLuint getTextureUnitID() const { return texUnitID; }
};

