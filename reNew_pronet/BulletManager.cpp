#include "BulletManager.h"

void BulletManager::init(const BulletCreateInfo& _info, const pronet::poolObject_shared_ptr<Object, 6>& _object
	, const pronet::poolObject_shared_ptr<Shader, 6>& _shader, const pronet::poolObject_shared_ptr<Texture, 6>& _texture)
{
	instance_.init(_info.id_, _info.coef_, _info.interval_, _info.rad_, _info.speed_);
	object_ = _object;
	shader_ = _shader;
	texture_ = _texture;
}

void BulletManager::reset()
{
	instance_.reset();
	object_.reset();
	shader_.reset();
}

void BulletManager::run(glm::vec2& _pos, bool _gen_stat)
{
	instance_.execute(_pos, _gen_stat);
	shader_->use();
	texture_->exe(shader_->getTextureUnitID());
	for (auto& a : instance_.bullet_array_) {
		pronet::updateUniformBlock(GAME_BULLET_DATA_PARAMETER, &a.param);
		for (auto& b : a.bullets_) {
			pronet::updateUniformBlock(GAME_BULLET_PARAMETER, &b);
			object_->draw();
		}
	}
}
