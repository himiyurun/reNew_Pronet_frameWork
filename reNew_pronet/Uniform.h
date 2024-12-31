#pragma once
#include <iostream>
#include <memory>
#include <glad/glad.h>

typedef struct {
	GLfloat size[2];			//	ウインドウのサイズ
	GLfloat nowPosition[2];		//	マウスの現在の位置
	GLfloat lastPosition[2];	//	マウスの図形をつかんだときの位置（つかんでいないときは0）
	GLfloat scale;				//	ウインドウのスケール
} WindowParam;					//	ウインドウの各種パラメーターを格納する構造体

struct Structure2v_Param {
	GLfloat location[2];	//	オブジェクトの位置
	GLfloat rotate;			//	オブジェクトの角度

	Structure2v_Param() : location{ 0.0f, 0.0f }, rotate(0.0f) {}
};

namespace pronet {
	class Uniform
	{
		struct UniformBuffer {
			GLuint ubo;
			GLsizeiptr data_size;

			UniformBuffer(GLsizeiptr data_size, const void* data = nullptr, GLsizeiptr size = 1)
				: data_size(data_size)
			{
				glGenBuffers(1, &ubo);
				glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				glBufferData(GL_UNIFORM_BUFFER, data_size * size, data, GL_DYNAMIC_DRAW);
			}

			void Init(const void* data = nullptr, GLsizeiptr size = 1) {
				Clear();
				glGenBuffers(1, &ubo);
				glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				glBufferData(GL_UNIFORM_BUFFER, data_size * size, data, GL_DYNAMIC_DRAW);
			}

			void Clear() {
				if (!glIsBuffer(ubo))
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

		Uniform(const char* name, GLsizeiptr data_size, const void* data = nullptr, GLsizeiptr size = 1)
			: BlockName(name)
			, buffer(new UniformBuffer(data_size, data, size))
			, BlockID(0xffffffff)
		{
		}

		~Uniform() {

		}

		//	uboの再初期化を行う
		void Init(const void* data = nullptr, GLsizeiptr size = 1) {
			buffer->Init(data, size);
		}

		//	uniform_block と指定のインデックスのバッファをバインドする
		bool ub_binding(GLuint program, GLuint select) const {
			GLuint block_id(glGetUniformBlockIndex(program, BlockName));
			if (block_id == 0xffffffff) {
				return false;
			}
			glUniformBlockBinding(program, block_id, select);
			return true;
		}

		//	uboのパラメータの更新を行う
		void Update(const void* data, GLsizeiptr size = 1) {
			glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->data_size * size, data);
		}

		//	バッファをバインドする
		void bind(GLuint select) const {
			glBindBufferBase(GL_UNIFORM_BUFFER, select, buffer->ubo);
		}


	};
}

