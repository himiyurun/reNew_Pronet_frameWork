#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>


class Object
{
	GLuint vao;

	GLuint vbo;

protected:

	GLsizei vertexcount;

public:

	Object();

	~Object();

	void Init(GLint size, GLsizei vertexcount, const glm::vec2* vertex);

	virtual void bind() const {
		glBindVertexArray(vao);
	}

	virtual void excute() const {
		glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
	}

	void draw() const {
		bind();
		excute();
	}
};

