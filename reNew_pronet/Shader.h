#pragma once
#include <functional>
#include <glad/glad.h>

#include "readtxt.h"

#ifdef PRONET_2D
#define PNGL_INT			(0x00)
#define PNGL_FLOAT			(0x01)
#define PNGL_DOUBLE			(0x02)

#define PNGL_INT_VEC2		(0x03)
#define PNGL_FLOAT_VEC2		(0x04)
#define PNGL_DOUBLE_VEC2	(0x05)
#endif

struct bindUniform {
	union {
		uint32_t *paramu;
		int32_t *parami;
		float *paramf;
		double *paramd;
	};

};

struct ShaderMakeInfo {
	std::string vsrc;
	std::string fsrc;
};

class Shader
{
	GLuint program;

public:
	Shader();

	~Shader();

	GLboolean Init(const char* vsrc, const char* fsrc);

	void Reset();

	GLboolean ProgramInfo(GLuint program) const;

	GLboolean ShaderInfo(GLuint shader, const char* name) const;

	void use() const { glUseProgram(program); }

	[[nodiscard]]GLuint getProgram() const { return program; }
};

