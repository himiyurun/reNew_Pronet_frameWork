#pragma once
#include <iostream>
#include <memory>
#include <glad/glad.h>

namespace pronet {
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

		const char* BlockName;
		std::shared_ptr<UniformBuffer> buffer;
		GLuint BlockID;

	public:

		Uniform(const char* name, const T* data = nullptr, GLsizeiptr size = 1)
			: BlockName(name)
			, buffer(new UniformBuffer(data, size))
			, BlockID(0xffffffff)
		{
		}

		~Uniform() {

		}

		bool Init(GLuint program, const T* data = nullptr, GLsizeiptr size = 1) {
			buffer->Init(data, size);
			BlockID = glGetUniformBlockIndex(program, BlockName);
			if (BlockID == 0xffffffff) {
				return false;
			}
			glUniformBlockBinding(program, BlockID, 0);
			return true;
		}

		void Update(const T* data, GLsizeiptr size = 1) {
			glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T) * size, data);
		}

		void bind() const {
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, buffer->ubo);
		}
	};
}

