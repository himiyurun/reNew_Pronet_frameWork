#include "Object.h"

Object::Object()
	: vao(0)
	, vbo(0)
	, vertexcount(0)
{
}

Object::~Object()
{
	glDeleteVertexArrays(1, &vbo);
	glDeleteBuffers(1, &vbo);
}

void Object::Init(GLint size, GLsizei vertexcount, const glm::vec2* vertex)
{
	this->vertexcount = vertexcount;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexcount, vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
