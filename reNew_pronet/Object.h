#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "pronet_memory.h"
#include "ObjectPoolList.h"

//	2�����I�u�W�F�N�g�����ۂ̏����i�[����\����
//	vertexcount : ���_�̐�
//	vertex : 2D���_����
//	indexcount : ���_�C���f�b�N�X�̌�
//	index : ���_�C���f�b�N�X�̔z��|�C���^
struct ObjectInfo2v{
	GLboolean index_used;
	GLboolean texture_used;
	GLsizei vertexcount;
	pronet::poolArray_unique_ptr<glm::vec2> verts;
	pronet::poolArray_unique_ptr<glm::vec2> uv;
	GLsizei indexcount;
	pronet::poolArray_unique_ptr<uint32_t> index;

	void reset() {
		index_used = false;
		texture_used = false;
		vertexcount = 0;
		verts.reset();
		uv.reset();
		indexcount = 0;
		index.reset();
	}
};

void print_ObjectInfo2v(const ObjectInfo2v* const info);

//	�I�u�W�F�N�g�N���X
//	vao��vbo���Ǘ�����
class Object
{
	GLuint vao;

	GLuint vbo;

	GLuint ibo;

protected:

	GLsizei vertexcount;

	GLsizei indexcount;

	GLboolean index_used;

public:

	Object();

	~Object();

	void init(GLint size, const ObjectInfo2v *const info, GLboolean index_used, GLboolean texture_used);
	void init(GLint size, GLsizei vertcount, const glm::vec2* vertex, GLsizei indcount, const GLuint* index);

	void reset();

	virtual void bind() const {
		glBindVertexArray(vao);
	}

	virtual void excute() const {
		if (index_used) {
			glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
		}
	}

	void draw() const {
		bind();
		excute();
	}

	void draw(GLenum _param) const {
		bind();
		if (index_used) {
			glDrawElements(_param, indexcount, GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(_param, 0, vertexcount);
		}
	}
};

