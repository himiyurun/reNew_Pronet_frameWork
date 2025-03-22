#pragma once
#include <iostream>
#include <array>
#include <memory>
#include <cassert>
#include <glad/glad.h>
#include <glm/glm.hpp>

typedef struct {
	GLfloat size[2];			//	�E�C���h�E�̃T�C�Y
	GLfloat nowPosition[2];		//	�}�E�X�̌��݂̈ʒu
	GLfloat lastPosition[2];	//	�}�E�X�̐}�`�����񂾂Ƃ��̈ʒu�i����ł��Ȃ��Ƃ���0�j
	GLfloat scale;				//	�E�C���h�E�̃X�P�[��
} WindowParam;					//	�E�C���h�E�̊e��p�����[�^�[���i�[����\����

struct Structure2vParam {
	GLfloat location[2];	//	�I�u�W�F�N�g�̈ʒu
	GLfloat rotate;			//	�I�u�W�F�N�g�̊p�x

	Structure2vParam() : location{ 0.0f, 0.0f }, rotate(0.0f) {}
};

typedef struct {
	GLfloat position[2];	//	�v���C���[�̈ʒu
	GLfloat rotate;			//	�v���C���[�̉�]�p
	GLuint motion_id;		//	�v���C���[�̃��[�V������ID
	GLboolean intersect;
}Player2vParam;

typedef struct {
	glm::vec2 position_;
	float degrees_;
}BulletParam;

enum ShaderBlock {
	PNGL_WINDOW_PARAM,
	PN_GAME_STRUCTURE_PARAM,
	PN_GAME_PLAYER_PARAM,
	GAME_BULLET_PARAM,
	SHADER_BLOCK_SIZE,
	SHADER_BLOCK_INFO_END
};

#define PRONET_GL_WINDOW_PARAMETER			PNGL_WINDOW_PARAM
#define PRONET_GAME_STRUCTURE_PARAMETER		PN_GAME_STRUCTURE_PARAM
#define PRONET_GAME_PLAYER_PARAMETER		PN_GAME_PLAYER_PARAM
#define GAME_BULLET_PARAMETER				GAME_BULLET_PARAM

namespace pronet {
	class Uniform
	{
		struct UniformBuffer {
			GLuint ubo;
			GLsizeiptr data_size;

			UniformBuffer(GLsizeiptr _data_size, const void* data = nullptr, GLsizeiptr size = 1)
				: data_size(_data_size)
			{
				glGenBuffers(1, &ubo);
				glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				glBufferData(GL_UNIFORM_BUFFER, data_size * size, data, GL_DYNAMIC_DRAW);
			}

			void Init(GLsizeiptr _data_size, const void* data = nullptr, GLsizeiptr size = 1) {
				Clear();
				data_size = _data_size;
				glGenBuffers(1, &ubo);
				glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				glBufferData(GL_UNIFORM_BUFFER, data_size * size, data, GL_DYNAMIC_DRAW);
			}

			void Clear() {
				if (!glIsBuffer(ubo))
					glDeleteBuffers(1, &ubo);
				data_size = 0;
			}

			~UniformBuffer() {
				Clear();
			}
		};

		const char* BlockName;
		std::shared_ptr<UniformBuffer> buffer;
		GLuint BlockID;

	public:

		Uniform(const char* name = nullptr, GLsizeiptr data_size = 0, const void* data = nullptr, GLsizeiptr size = 1)
			: BlockName(name)
			, buffer((name == nullptr && data_size == 0) ? nullptr : new UniformBuffer(data_size, data, size))
			, BlockID(0xffffffff)
		{}

		~Uniform() {}

		//	ubo�̍ď��������s��
		void Init(const char* name, GLsizeiptr _data_size, const void* data = nullptr, GLsizeiptr size = 1) {
			BlockName = name;
			if (buffer) buffer.reset();
			buffer = std::make_shared<UniformBuffer>(_data_size, data, size);
		}

		//	uniform_block �Ǝw��̃C���f�b�N�X�̃o�b�t�@���o�C���h����
		bool ub_binding(GLuint program, GLuint &block_ID) const {
			assert(buffer && "assertion Error : uniform_buffer have not initlize!");
			GLuint block_id(glGetUniformBlockIndex(program, BlockName));
			if (block_id == 0xffffffff) {
				return false;
			}
			glUniformBlockBinding(program, block_id, block_id);
			block_ID = block_id;
			return true;
		}

		//	ubo�̃p�����[�^�̍X�V���s��
		void Update(const void* data, GLsizeiptr size = 1) {
			assert(buffer && "assertion Error : uniform_buffer have not initlize!");
			glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->data_size * size, data);
		}

		//	�o�b�t�@���o�C���h����
		void bind(GLuint select) const {
			assert(buffer && "assertion Error : uniform_buffer have not initlize!");
			glBindBufferBase(GL_UNIFORM_BUFFER, select, buffer->ubo);
		}
	};

	//	UniformBufferObject ��z��ŊǗ�
	static std::array<Uniform, SHADER_BLOCK_SIZE> unf;

	//	UnfiromBlockObject ������������
	void initUniformBlock();
	//	���� UniformBlock �����݂���̂����m�F����
	void getBlockBindInfo(GLuint program, size_t buf[SHADER_BLOCK_SIZE]);
	//	�v���C���[�̃p�����[�^�[�� UniformBlock	���A�b�v�f�[�g����
	void updatePlayer2vUniformParam(const Player2vParam* const playerParam);
	//	�w�肵�� UniformBlock ���A�b�v�f�[�g����
	void updateGameObjectUniformParam(const Structure2vParam* strParam);
	//	�A�v���P�[�V�����Ɋ֘A����p�����[�^�[���X�V����
	void updateApplicationUniformParam(const WindowParam* windowParam);
	//	�w�肳�ꂽ���j�t�H�[���u���b�N�̃p�����[�^�[���X�V
	void updateUniformBlock(ShaderBlock _param, const void* const _ptr);
	//	UniformObject ���A�b�v�f�[�g���ăo�C���h����
	void bindUniformObject(const size_t buf[SHADER_BLOCK_SIZE]);
}