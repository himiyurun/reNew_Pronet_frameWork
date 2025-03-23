#include "Object.h"

Object::Object()
	: vao(0)
	, vbo(0)
	, ibo(0)
	, tbo(0)
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexcount, &info->verts[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	if (this->index_used) {
		indexcount = info->indexcount;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexcount, &info->index[0], GL_STATIC_DRAW);
	}

	if (texture_used) {
		glGenBuffers(1, &tbo);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexcount, &info->uv[0], GL_STATIC_DRAW);

		glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}
}

void Object::init(GLint size, GLsizei vertcount, const glm::vec2* vertex, GLsizei indcount, const GLuint* index, const glm::vec2* uv)
{
	index_used = true;
	vertexcount = vertcount;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexcount, vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	if (this->index_used) {
		indexcount = indcount;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::vec2) * indexcount, index, GL_STATIC_DRAW);
	}

	if (uv) {
		glGenBuffers(1, &tbo);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexcount, uv, GL_STATIC_DRAW);

		glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
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