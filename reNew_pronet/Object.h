#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>

//	2�����I�u�W�F�N�g�����ۂ̏����i�[����\����
//	vertexcount : ���_�̐�
//	vertex : 2D���_����
//	indexcount : ���_�C���f�b�N�X�̌�
//	index : ���_�C���f�b�N�X�̔z��|�C���^
struct ObjectInfo2v {
	GLsizei vertexcount;
	const glm::vec2* vertex;
	GLsizei indexcount;
	const GLuint* index;
};

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

	void Init(GLint size, ObjectInfo2v *info, GLboolean index_used);

	void Reset();

	virtual void bind() const {
		glBindVertexArray(vao);
	}

	virtual void excute() const {
		if (index_used)
			glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, nullptr);
		else
			glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
	}

	void draw() const {
		bind();
		excute();
	}
};

