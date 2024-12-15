#pragma once
#include <iostream>
#include <memory>

#include <glad/glad.h>

/*
* シェーダのUniformブロックリスト
* 
window {
	vec2 size;
	float scale;
	vec2 mousePos;
}

object2v {
	vec2 location;
}

Function2v {
	vec2 func;
}
* 
*/

template<typename T>
class Uniform
{
	struct UniformBuffer {
		GLuint ubo;

		UniformBuffer(const T* data = nullptr, GLsizeiptr size = 1) {
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * size, data, GL_DYNAMIC_DRAW);
		}

		void Init(const T* data = nullptr, GLsizeiptr size = 1) {
			Clear();
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * size, data, GL_DYNAMIC_DRAW);
		}

		void Clear() {
			glDeleteBuffers(1, &ubo);
		}

		~UniformBuffer() {
			Clear();
		}
	};

	std::shared_ptr<UniformBuffer> buffer;
	GLuint BlockID;

public:

	Uniform(GLuint program, GLuint BlockId, const T* data = nullptr, GLsizeiptr size = 1)
		: buffer(new UniformBuffer(data, size))
		, BlockID(BlockId)
	{
		glUniformBlockBinding(program, BlockID, BlockID);
	}

	~Uniform() {

	}

	void Init(GLuint program, GLuint BlockId, const T* data = nullptr, GLsizeiptr size = 1) {
		buffer->Init(data, size);
		BlockID = BlockId;
		glUniformBlockBinding(program, BlockID, BlockID);
	}

	void Updata(const T* data, GLsizeiptr size = 1) {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T) * size, data);
	}

	void bind() const {
		glBindBufferBase(GL_UNIFORM_BUFFER, BlockID, buffer->ubo);
	}
};

