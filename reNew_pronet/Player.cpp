#include "Player.h"

pronet::Player::Player(const GLint size, Player2vCreateInfo* const info)
{
	reset();
	if (info) {
		init(size, info);
	}
}

pronet::Player::~Player()
{
}

void pronet::Player::init(const GLint size, Player2vCreateInfo* const info)
{
	if (!info) { throw std::runtime_error("Player initialize infomation is null : Player.init()"); }
	obj.init(size, &info->_obj, info->_obj.index_used, info->_obj.texture_used);
	shd.init(info->_shd.vsrc.c_str(), info->_shd.fsrc.c_str());

	col_pos[0] = info->col_pos[0]; col_pos[1] = info->col_pos[1];
	col_size[0] = info->col_size[0]; col_size[1] = info->col_size[1];
	param.position[0] = info->position[0]; param.position[1] = info->position[1];
	param.rotate = info->rotate;
	param.motion_id = 0;
	param.intersect = false;

	info->_obj.reset();
}

void pronet::Player::reset()
{
	obj.reset();
	shd.reset();
	col_pos[0] = 0.f; col_pos[1] = 0.f;
	col_size[0] = 0.f; col_size[1] = 0.f;
	param.position[0] = 0.f; param.position[1] = 0.f;
	param.rotate = 0.f;
	param.motion_id = 0;
	param.intersect = false;
}

void pronet::Player::draw() const
{
	shd.use();
	obj.draw();
}
