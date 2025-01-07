#pragma once
#include "Object.h"
#include "Shader.h"
#include "Uniform.h"

namespace pronet {
	typedef struct {
		ObjectInfo2v _obj;
		ShaderMakeInfo _shd;
		GLfloat col_pos[2];
		GLfloat col_size[2];
		GLfloat position[2];
		GLfloat rotate;
	}Player2vCreateInfo;

	class Player
	{
		Object obj;
		Shader shd;

		GLfloat col_pos[2];
		GLfloat col_size[2];
	public:
		Player2vParam param;
	public:
		Player(const GLint size = 2, Player2vCreateInfo* const info = nullptr);
		~Player();

		void init(const GLint size, Player2vCreateInfo* const info);
		void reset();

		void draw() const;
		
		[[nodiscard]] const Player2vParam* parameter() const { return &param; }

	private:

	};
}

