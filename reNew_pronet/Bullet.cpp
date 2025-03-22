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
		update(a.pos_, a.beg_rad_, a.radius_, a.bullets_);
	}
	if (_gen_stat) {
		Bullet_Data buf;
		generate(buf.pos_, buf.beg_rad_, buf.radius_, buf.bullets_);
		bullet_array_.emplace_back(buf);
	}
	else {
		if (bullet_array_.empty())
			return;
	}

	if (bullet_array_[0].radius_ > 16.f) {
		bullet_array_.erase(bullet_array_.begin());
	}
}

void Bullet::generate(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets)
{
	_pos = glm::vec2(0.f);
	_beg_rad = 0.f;
	_radius = 0.f;
	_bullets.resize(coef_);
	float nowrad(0.f);
	const float everyrad(360.f / (float)coef_);
	for (auto& a : _bullets) {
		a.degrees_ = nowrad;
		nowrad += everyrad;
	}
}

void Bullet::update(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets)
{
	_radius += speed_;
	for (auto& a : _bullets) {
		a.position_ += speed_ * glm::vec2(cos(a.degrees_), sin(a.degrees_));
	}
}
