#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "ObjectPoolList.h"

//	2次元オブジェクトを作る際の情報を格納する構造体
//	vertexcount : 頂点の数
//	vertex : 2D頂点属性
//	indexcount : 頂点インデックスの個数
//	index : 頂点インデックスの配列ポインタ
struct ObjectInfo2v{
	GLsizei vertexcount;
	pronet::PoolArray<glm::vec2> verts;
	pronet::PoolArray<glm::vec2> uv;
	GLsizei indexcount;
	pronet::PoolArray<uint32_t> index;
	uint8_t shader_index;
};

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

	void init(GLint size, ObjectInfo2v *info, GLboolean index_used);

	void reset();

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

