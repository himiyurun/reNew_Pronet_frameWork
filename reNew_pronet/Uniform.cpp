#include "Uniform.h"
using namespace pronet;

void pronet::initUniformBlock()
{
	unf[PNGL_WINDOW_PARAM].Init("window", sizeof(WindowParam));
	unf[PN_GAME_STRUCTURE_PARAM].Init("structure", sizeof(Structure2vParam));
	unf[PN_GAME_PLAYER_PARAM].Init("player", sizeof(Player2vParam));
	unf[GAME_BULLET_DATA_PARAM].Init("bulletData", sizeof(BulletDataParam));
	unf[GAME_BULLET_PARAM].Init("bullet", sizeof(BulletParam));
}

void pronet::getBlockBindInfo(GLuint program, size_t buf[SHADER_BLOCK_SIZE])
{
	GLuint block_id(0);
	GLuint max_block_id(block_id);
	for (size_t i = 0; i < SHADER_BLOCK_SIZE; i++) {
		if (unf[i].ub_binding(program, block_id)) {
			buf[block_id] = i;
			if (block_id > max_block_id)
				max_block_id = block_id;
		}
	}
	if (max_block_id < SHADER_BLOCK_SIZE) buf[max_block_id + 1] = SHADER_BLOCK_INFO_END;
}

void pronet::updatePlayer2vUniformParam(const Player2vParam* const playerParam)
{
	unf[PN_GAME_PLAYER_PARAM].Update(playerParam);
}

void pronet::updateGameObjectUniformParam(const Structure2vParam* strParam)
{
	unf[PN_GAME_STRUCTURE_PARAM].Update(strParam);
}

void pronet::updateApplicationUniformParam(const WindowParam* windowParam)
{
	unf[PNGL_WINDOW_PARAM].Update(windowParam);
}

void pronet::updateUniformBlock(ShaderBlock _param, const void* const _ptr)
{
	unf[_param].Update(_ptr);
}

void pronet::bindUniformObject(const size_t buf[SHADER_BLOCK_SIZE])
{
	for (size_t i = 0; i < SHADER_BLOCK_SIZE; i++) {
		if (buf[i] == SHADER_BLOCK_INFO_END) break;
		unf.at(buf[i]).bind(i);
	}
}