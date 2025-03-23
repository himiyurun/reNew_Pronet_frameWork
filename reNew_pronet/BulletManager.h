#pragma once
#include "pronet_memory.h"
#include "Bullet.h"
#include "Object.h"
#include "Shader.h"
#include "Texture.h"

typedef struct {
	const char* id_;		//	’e–‹‚É•t‚¯‚é–¼‘O
	uint32_t coef_;			//	’e–‹‚ÌŒÂ”
	uint32_t interval_;		//	’e–‹‚Ì”­Ë‚·‚é•p“x
	float rad_;				//	’e–‹‚Ì”­Ë‚·‚éŠp“x
	float speed_;			//	’e–‹‚Ì‘¬“x
}BulletCreateInfo;

class BulletManager
{
	Bullet instance_;
	pronet::poolObject_shared_ptr<Object, 6> object_;
	pronet::poolObject_shared_ptr<Shader, 6> shader_;
	pronet::poolObject_shared_ptr<Texture, 6> texture_;
public:
	BulletManager(const BulletCreateInfo& _info = BulletCreateInfo()
		, const pronet::poolObject_shared_ptr<Object, 6>& _object = pronet::poolObject_shared_ptr<Object, 6>()
		, const pronet::poolObject_shared_ptr<Shader, 6>& _shader = pronet::poolObject_shared_ptr<Shader, 6>()
		, const pronet::poolObject_shared_ptr<Texture, 6>& _texture = pronet::poolObject_shared_ptr<Texture, 6>())
		: instance_(_info.id_, _info.coef_, _info.interval_, _info.rad_, _info.speed_)
		, object_(_object), shader_(_shader), texture_(_texture)
	{

	}

	~BulletManager() {}

	void init(const BulletCreateInfo& _info, const pronet::poolObject_shared_ptr<Object, 6>& _object
		, const pronet::poolObject_shared_ptr<Shader, 6>& _shader, const pronet::poolObject_shared_ptr<Texture, 6>& _texture = pronet::poolObject_shared_ptr<Texture, 6>());

	void reset();

	void run(glm::vec2& _pos, bool _gen_stat);
};

