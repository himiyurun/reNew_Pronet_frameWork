#include "Shader.h"

Shader::Shader()
	: program(0)
{
}

Shader::~Shader()
{
}

GLboolean Shader::Init(const char* vsrc, const char* fsrc)
{
	program = glCreateProgram();
	if (vsrc) {
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		readtxt buf(vsrc);
		const char* src = buf.c_ptr();
		glShaderSource(vobj, 1, &src, nullptr);
		glCompileShader(vobj);

		if (ShaderInfo(vobj, "Vertex Shader")) {
			glAttachShader(program, vobj);
		}
		glDeleteShader(vobj);
	}
	if (fsrc) {
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		readtxt buf(fsrc);
		const char* src = buf.c_ptr();
		glShaderSource(fobj, 1, &src, nullptr);
		glCompileShader(fobj);

		if (ShaderInfo(fobj, "Fragment Shader")) {
			glAttachShader(program, fobj);
		}
		glDeleteShader(fobj);
	}

	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program, 0, "fragment");

	glLinkProgram(program);

	if (!ProgramInfo(program)) {
		glDeleteProgram(program);
		program = 0;
		return false;
	}

	return true;
}

void Shader::Reset()
{
	if (program != 0) {
		glDeleteProgram(program);
	}
}

GLboolean Shader::ProgramInfo(GLuint program) const
{
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		std::cerr << "Shader Program Link failed" << std::endl;
	}

	int32_t bufsize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufsize);
	if (bufsize > 1) {
		std::vector<GLchar> info(bufsize);
		GLsizei length;
		glGetProgramInfoLog(program, bufsize, &length, info.data());
		std::cerr << &info[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

GLboolean Shader::ShaderInfo(GLuint shader, const char* name) const
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		std::cerr << "Shader Program Link failed" << std::endl;
	}

	int32_t bufsize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufsize);
	if (bufsize > 1) {
		std::vector<GLchar> info(bufsize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufsize, &length, info.data());
		std::cerr << &info[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}
