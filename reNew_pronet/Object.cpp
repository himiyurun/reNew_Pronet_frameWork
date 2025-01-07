#include "Object.h"

Object::Object()
	: vao(0)
	, vbo(0)
	, ibo(0)
	, vertexcount(0)
	, indexcount(0)
	, index_used(false)
{
}

Object::~Object()
{
	reset();
}

void Object::init(GLint size, const ObjectInfo2v *const info, GLboolean index_used, GLboolean texture_used)
{
	vertexcount = info->vertexcount;
	this->index_used = index_used;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexcount, info->verts->data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	if (this->index_used) {
		indexcount = info->indexcount;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::vec2) * indexcount, info->index->data, GL_STATIC_DRAW);
	}
}

void Object::reset()
{
	if (vao != 0) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	if (vbo != 0) {
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}
	if (ibo != 0) {
		glDeleteBuffers(1, &ibo);
		ibo = 0;
	}
		
	vertexcount = 0;
	indexcount = 0;
	index_used = false;
}

void print_ObjectInfo2v(const ObjectInfo2v* const info)
{
	std::cout << "vertex_count : " << info->vertexcount << std::endl;
	std::cout << "index_count : " << info->indexcount << std::endl;
	for (int i = 0; i < info->vertexcount; i++) {
		std::cout << "verts " << i << " : " << info->verts[i].x << " " << info->verts[i].y << std::endl;
	}
	for (int i = 0; i < info->vertexcount; i++) {
		std::cout << "uv " << i << " : " << info->uv[i].x << " " << info->uv[i].y << std::endl;
	}
	for (int i = 0; i < info->vertexcount; i++) {
		std::cout << "index " << i << " : " << info->index[i] << std::endl;
	}
}