#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

#include "pnTlsf.h"
#include "Uniform.h"
#include "pnMath.h"

/*
* 弾幕の管理をするクラス
* 弾幕の管理をするクラス
*/

typedef struct {
	BulletDataParam param;
	std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>> bullets_;	//	各弾幕の位置
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
	uint32_t coef_;		//	弾幕の個数
	uint32_t interval_;	//	弾幕の発射する頻度
	float rad_;			//	弾幕の発射する角度
	float speed_;		//	弾幕の速度
public:
	std::vector<Bullet_Data, pnTlsfInsertSTLtype<Bullet_Data>> bullet_array_;	//	弾幕のインスタンス
	const char* bullet_id_;	//	弾幕名
public:
	//	コンストラクタ
	//	coef : 1度に発射する弾幕の個数
	//	interval : 弾幕の発射する頻度
	//	rad : 弾幕の発射する角度
	//	speed : 弾幕の速度	
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

