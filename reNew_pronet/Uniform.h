#pragma once
#include <iostream>
#include <array>
#include <memory>
#include <cassert>
#include <glad/glad.h>
#include <glm/glm.hpp>

typedef struct {
	GLfloat size[2];			//	ウインドウのサイズ
	GLfloat nowPosition[2];		//	マウスの現在の位置
	GLfloat lastPosition[2];	//	マウスの図形をつかんだときの位置（つかんでいないときは0）
	GLfloat scale;				//	ウインドウのスケール
} WindowParam;					//	ウインドウの各種パラメーターを格納する構造体

struct Structure2vParam {
	GLfloat location[2];	//	オブジェクトの位置
	GLfloat rotate;			//	オブジェクトの角度

	Structure2vParam() : location{ 0.0f, 0.0f }, rotate(0.0f) {}
};

typedef struct {
	GLfloat position[2];	//	プレイヤーの位置
	GLfloat rotate;			//	プレイヤーの回転角
	GLuint motion_id;		//	プレイヤーのモーションのID
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

		//	uboの再初期化を行う
		void Init(const char* name, GLsizeiptr _data_size, const void* data = nullptr, GLsizeiptr size = 1) {
			BlockName = name;
			if (buffer) buffer.reset();
			buffer = std::make_shared<UniformBuffer>(_data_size, data, size);
		}

		//	uniform_block と指定のインデックスのバッファをバインドする
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

		//	uboのパラメータの更新を行う
		void Update(const void* data, GLsizeiptr size = 1) {
			assert(buffer && "assertion Error : uniform_buffer have not initlize!");
			glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->data_size * size, data);
		}

		//	バッファをバインドする
		void bind(GLuint select) const {
			assert(buffer && "assertion Error : uniform_buffer have not initlize!");
			glBindBufferBase(GL_UNIFORM_BUFFER, select, buffer->ubo);
		}
	};

	//	UniformBufferObject を配列で管理
	static std::array<Uniform, SHADER_BLOCK_SIZE> unf;

	//	UnfiromBlockObject を初期化する
	void initUniformBlock();
	//	その UniformBlock が存在するのかを確認する
	void getBlockBindInfo(GLuint program, size_t buf[SHADER_BLOCK_SIZE]);
	//	プレイヤーのパラメーターの UniformBlock	をアップデートする
	void updatePlayer2vUniformParam(const Player2vParam* const playerParam);
	//	指定した UniformBlock をアップデートする
	void updateGameObjectUniformParam(const Structure2vParam* strParam);
	//	アプリケーションに関連するパラメーターを更新する
	void updateApplicationUniformParam(const WindowParam* windowParam);
	//	指定されたユニフォームブロックのパラメーターを更新
	void updateUniformBlock(ShaderBlock _param, const void* const _ptr);
	//	UniformObject をアップデートしてバインドする
	void bindUniformObject(const size_t buf[SHADER_BLOCK_SIZE]);
}