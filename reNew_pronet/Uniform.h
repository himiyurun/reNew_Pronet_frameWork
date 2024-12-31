#pragma once
#include <iostream>
#include <memory>
#include <glad/glad.h>

typedef struct {
	GLfloat size[2];			//	�E�C���h�E�̃T�C�Y
	GLfloat nowPosition[2];		//	�}�E�X�̌��݂̈ʒu
	GLfloat lastPosition[2];	//	�}�E�X�̐}�`�����񂾂Ƃ��̈ʒu�i����ł��Ȃ��Ƃ���0�j
	GLfloat scale;				//	�E�C���h�E�̃X�P�[��
} WindowParam;					//	�E�C���h�E�̊e��p�����[�^�[���i�[����\����

struct Structure2v_Param {
	GLfloat location[2];	//	�I�u�W�F�N�g�̈ʒu
	GLfloat rotate;			//	�I�u�W�F�N�g�̊p�x

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

		//	ubo�̍ď��������s��
		void Init(const void* data = nullptr, GLsizeiptr size = 1) {
			buffer->Init(data, size);
		}

		//	uniform_block �Ǝw��̃C���f�b�N�X�̃o�b�t�@���o�C���h����
		bool ub_binding(GLuint program, GLuint select) const {
			GLuint block_id(glGetUniformBlockIndex(program, BlockName));
			if (block_id == 0xffffffff) {
				return false;
			}
			glUniformBlockBinding(program, block_id, select);
			return true;
		}

		//	ubo�̃p�����[�^�̍X�V���s��
		void Update(const void* data, GLsizeiptr size = 1) {
			glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->data_size * size, data);
		}

		//	�o�b�t�@���o�C���h����
		void bind(GLuint select) const {
			glBindBufferBase(GL_UNIFORM_BUFFER, select, buffer->ubo);
		}


	};
}

