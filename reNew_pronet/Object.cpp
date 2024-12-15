#include "Object.h"

Object::Object()
	: vao(0)
	, vbo(0)
	, ibo(0)
	, vertexcount(0)
	, indexcount(0)
{
}

Object::~Object()
{
}

void Object::Init(GLint size, ObjectInfo2v* info, GLboolean index_used)
{
	vertexcount = info->vertexcount;
	this->index_used = index_used;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexcount, info->verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	if (this->index_used) {
		indexcount = info->indexcount;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::vec2) * indexcount, info->index, GL_STATIC_DRAW);
	}
}

void Object::Reset()
{
	glDeleteVertexArrays(1, &vbo);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	vertexcount = 0;
}
