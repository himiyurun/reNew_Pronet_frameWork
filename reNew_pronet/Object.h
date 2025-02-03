#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "pronet_memory.h"
#include "ObjectPoolList.h"

//	2次元オブジェクトを作る際の情報を格納する構造体
//	vertexcount : 頂点の数
//	vertex : 2D頂点属性
//	indexcount : 頂点インデックスの個数
//	index : 頂点インデックスの配列ポインタ
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

//	オブジェクトクラス
//	vaoやvboを管理する
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

