#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

#include "pnTlsf.h"
#include "Uniform.h"
#include "pnMath.h"

/*
* ’e–‹‚ÌŠÇ—‚ğ‚·‚éƒNƒ‰ƒX
* ’e–‹‚ÌŠÇ—‚ğ‚·‚éƒNƒ‰ƒX
*/

typedef struct {
	BulletDataParam param;
	std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>> bullets_;	//	Še’e–‹‚ÌˆÊ’u
}Bullet_Data;

constexpr glm::vec2 bulletAngleVertex[] = {
	{  0.5f,  0.5f },
	{ -0.5f,  0.5f },
	{ -0.5f, -0.5f },
	{  0.5f, -0.5f }
};

constexpr glm::vec2 bulletAngleUv[] = {
	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f }
};

constexpr uint32_t bulletAngleIndex[] = {
	0, 1, 2,
	0, 3, 2
};

class Bullet
{
	uint32_t coef_;		//	’e–‹‚ÌŒÂ”
	uint32_t interval_;	//	’e–‹‚Ì”­Ë‚·‚é•p“x
	float rad_;			//	’e–‹‚Ì”­Ë‚·‚éŠp“x
	float speed_;		//	’e–‹‚Ì‘¬“x
public:
	std::vector<Bullet_Data, pnTlsfInsertSTLtype<Bullet_Data>> bullet_array_;	//	’e–‹‚ÌƒCƒ“ƒXƒ^ƒ“ƒX
	const char* bullet_id_;	//	’e–‹–¼
public:
	//	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	//	coef : 1“x‚É”­Ë‚·‚é’e–‹‚ÌŒÂ”
	//	interval : ’e–‹‚Ì”­Ë‚·‚é•p“x
	//	rad : ’e–‹‚Ì”­Ë‚·‚éŠp“x
	//	speed : ’e–‹‚Ì‘¬“x	
	Bullet(const char* _bullet_id = nullptr, uint32_t _coef = 0, uint32_t _interval = 0, float _rad = 0.f, float _speed = 0.f)
		: coef_(_coef), interval_(_interval)
		, rad_(_rad), speed_(_speed)
		, bullet_id_(_bullet_id)
	{
	}

	virtual ~Bullet() {}

	void init(const char* _bullet_id, uint32_t _coef = 0, uint32_t _interval = 0, float _rad = 0.f, float _speed = 0.f);

	void reset();

	virtual void execute(glm::vec2& _pos, bool _gen_stat);

	virtual void generate(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets);

	virtual void update(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets);

	[[nodiscard]] const char* id() const { return bullet_id_; }
};

