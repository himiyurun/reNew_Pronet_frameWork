#include "Bullet.h"

void Bullet::init(const char* _bullet_id, uint32_t _coef, uint32_t _interval, float _rad, float _speed)
{
	coef_ = _coef;
	interval_ = _interval;
	rad_ = _rad;
	speed_ = _speed;
	bullet_id_ = _bullet_id;
}

void Bullet::reset()
{
	coef_ = 0;
	interval_ = 0;
	rad_ = 0;
	speed_ = 0;
	bullet_array_.clear();
	bullet_id_ = nullptr;
}

void Bullet::execute(glm::vec2& _pos, bool _gen_stat)
{
	for (auto& a : bullet_array_) {
		update(a.param.pos_, a.param.beg_rad_, a.param.radius_, a.bullets_);
	}
	if (_gen_stat) {
		Bullet_Data buf;
		generate(buf.param.pos_, buf.param.beg_rad_, buf.param.radius_, buf.bullets_);
		buf.param.pos_ = _pos;
		bullet_array_.emplace_back(buf);
	}
	else {
		if (bullet_array_.empty())
			return;
	}

	if (bullet_array_[0].param.radius_ > 16.f) {
		bullet_array_.erase(bullet_array_.begin());
	}
}

void Bullet::generate(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets)
{
	_beg_rad = 0.f;
	_radius = 0.f;
	_bullets.resize(coef_);
	float nowrad(_beg_rad);
	const float everyrad(360.f / (float)coef_);
	for (auto& a : _bullets) {
		a.degrees_ = nowrad;
		nowrad += everyrad;
	}
}

void Bullet::update(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets)
{
	_radius += speed_;
	float rad(0.f);
	for (auto& a : _bullets) {
		rad = pronet::radiansf(a.degrees_);
		a.position_ += speed_ * glm::vec2(cos(rad), sin(rad));
	}
}
